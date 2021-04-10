/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/SkaaracTheBloodborn.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {

    class CallOfTheSkullThrone : public CommandAbility {
    public:
        explicit CallOfTheSkullThrone(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    CallOfTheSkullThrone::CallOfTheSkullThrone(Unit *source) :
            CommandAbility(source, "Call of the Skull Throne", 12, 12, Phase::Charge) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {KHORNE};
    }

    bool CallOfTheSkullThrone::apply(Unit *target) {
        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(KHORNE)) {
                unit->buffReroll(Attribute::Charge_Distance, Rerolls::Failed, defaultDuration());
            }
        }
        return true;
    }

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 420;

    struct TableEntry {
        int m_move;
        int m_bloodToWound;
        int m_bladeAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 7, 10, 14, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 3, 8},
                    {9,  3, 7},
                    {8,  4, 6},
                    {7,  4, 5},
                    {6,  5, 4}
            };

    bool SkaaracTheBloodborn::s_registered = false;

    SkaaracTheBloodborn::SkaaracTheBloodborn(SlaughterHost host, bool isGeneral) :
            KhorneBase("Skaarac the Bloodborn", 10, g_wounds, 10, 4, false) {
        m_keywords = {CHAOS, KHORNE, MONSTER, HERO, SKAARAC_THE_BLOODBORN};
        m_weapons = {&m_burningBlood, &m_blades, &m_hooves};
        m_battleFieldRole = Role::Leader_Behemoth;

        setSlaughterHost(host);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_burningBlood);
        model->addMeleeWeapon(&m_blades);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);

        s_globalCastMod.connect(this, &SkaaracTheBloodborn::infernalIron, &m_infernalIronSlot);
        s_globalBraveryMod.connect(this, &SkaaracTheBloodborn::toweringHorror, &m_toweringHorrowSlot);

        m_commandAbilities.push_back(std::make_unique<CallOfTheSkullThrone>(this));

        m_points = g_pointsPerUnit;
    }

    SkaaracTheBloodborn::~SkaaracTheBloodborn() {
        m_infernalIronSlot.disconnect();
        m_toweringHorrowSlot.disconnect();
    }

    Unit *SkaaracTheBloodborn::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SkaaracTheBloodborn(host, general);
    }

    void SkaaracTheBloodborn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SkaaracTheBloodborn::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    SkaaracTheBloodborn::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skaarac the Bloodborn", factoryMethod);
        }
    }

    size_t SkaaracTheBloodborn::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void SkaaracTheBloodborn::onRestore() {
        // Restore table-driven attributes
        onWounded();
        KhorneBase::onRestore();
    }

    void SkaaracTheBloodborn::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_burningBlood.setToWound(g_damageTable[damageIndex].m_bloodToWound);
        m_blades.setAttacks(g_damageTable[damageIndex].m_bladeAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }

    int SkaaracTheBloodborn::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void SkaaracTheBloodborn::onEndCombat(PlayerId player) {
        KhorneBase::onEndCombat(player);
        if (m_lifeEaterHeal) {
            heal(Dice::RollD3());
            m_lifeEaterHeal = false;
        }
    }

    void SkaaracTheBloodborn::onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) {
        KhorneBase::onEnemyModelSlain(numSlain, enemyUnit, source);

        if (source == Wounds::Source::Weapon_Melee) {
            m_lifeEaterHeal = true;
        }
    }

    int SkaaracTheBloodborn::infernalIron(const Unit *caster) {
        if (caster && !isFriendly(caster) && (caster->hasKeyword(WIZARD)) && (distanceTo(caster) < 12.0)) {
            return -2;
        }
        return 0;
    }

    int SkaaracTheBloodborn::toweringHorror(const Unit *unit) {
        if (unit && !isFriendly(unit) && (distanceTo(unit) < 12.0)) {
            return -1;
        }
        return 0;
    }

} // namespace Khorne