/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/MazarallTheButcher.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 340;

    struct TableEntry {
        int m_move;
        int m_harrowRend;
        int m_bladeAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, -2, 5},
                    {10,  -2, 4},
                    {8,  -1, 3},
                    {6,  -1, 2},
                    {4,  0, 1}
            };

    bool MazarallTheButcher::s_registered = false;

    MazarallTheButcher::MazarallTheButcher(SlaughterHost host, bool isGeneral) :
            KhorneBase("Mazarall the Butcher", 12, g_wounds, 10, 3, false) {
        m_keywords = {CHAOS, DAEMON, KHORNE, MONSTER, HERO, MAZARALL_THE_BUTCHER};
        m_weapons = {&m_shieldWrath, &m_harrowMeat, &m_shieldBlades};
        m_battleFieldRole = Role::Leader_Behemoth;

        // The Ancyte Shield
        m_totalUnbinds = 1;

        setSlaughterHost(host);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_shieldWrath);
        model->addMeleeWeapon(&m_harrowMeat);
        model->addMeleeWeapon(&m_shieldBlades);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffRerollCommandAbility>(this, "The Butcher's Due", 18, 18, GamePhase::Hero,
                                                           Attribute::To_Wound_Melee, Rerolls::Ones,
                                                           Abilities::Target::SelfAndFriendly,
                                                           std::vector<Keyword>{KHORNE}));

        m_points = g_pointsPerUnit;
    }

    Unit *MazarallTheButcher::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new MazarallTheButcher(host, general);
    }

    void MazarallTheButcher::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MazarallTheButcher::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    MazarallTheButcher::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Mazarall the Butcher", factoryMethod);
        }
    }

    size_t MazarallTheButcher::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void MazarallTheButcher::onRestore() {
        // Restore table-driven attributes
        onWounded();
        m_harrowMeat.setAttacks(4);
        KhorneBase::onRestore();
    }

    void MazarallTheButcher::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_harrowMeat.setRend(g_damageTable[damageIndex].m_harrowRend);
        m_shieldBlades.setAttacks(g_damageTable[damageIndex].m_bladeAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }

    int MazarallTheButcher::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MazarallTheButcher::onCharged() {
        // Bloody Charge
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        for (auto unit : units) {
            if (Dice::RollD6() >= 4) {
                const Wounds wounds{0, Dice::RollD3(), Wounds::Source::Ability};
                unit->applyDamage(wounds, this);
                PLOG_INFO << name() << " Bloody Charge inflicted " << wounds.mortal << " mortal wounds on " << unit->name();
            }
        }
        KhorneBase::onCharged();
    }

    void MazarallTheButcher::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon,
                                                         const Wounds &weaponDamage) {
        KhorneBase::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        // Harrow Meat's Hunger
        if (weapon->name() == m_harrowMeat.name() && (numSlain > 0)) {
            m_harrowMeat.setAttacks(m_harrowMeat.attacks() + 1);
        }
    }

} // namespace Khorne