/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Rotigus.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 130;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 320;

    struct TableEntry {
        int m_rodToHit;
        int m_mawToWound;
        int m_delugeOfNurgle;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {2, 2, 4},
                    {3, 2, 5},
                    {3, 3, 5},
                    {4, 3, 6},
                    {4, 4, 6}
            };

    bool Rotigus::s_registered = false;

    Rotigus::Rotigus(PlagueLegion legion, Lore lore, bool isGeneral) :
            NurgleBase(legion,"Rotigus", 5, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_gnarlrod(Weapon::Type::Melee, "Gnarlrod", 3, 5, 2, 3, -1, 2),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 1, RAND_D3, 3, 2, -2, 2),
            m_nurglings(Weapon::Type::Melee, "Host of Nurglings", 1, 3, 5, 5, 0, 1) {
        m_keywords = {CHAOS, DAEMON, NURGLE, MONSTER, HERO, WIZARD, GREAT_UNCLEAN_ONE, ROTIGUS};
        m_weapons = {&m_gnarlrod, &m_fangedMaw, &m_nurglings};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalUnbinds = 2;
        m_totalSpells = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_gnarlrod);
        model->addMeleeWeapon(&m_fangedMaw);
        model->addMeleeWeapon(&m_nurglings);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *Rotigus::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Rotigus(legion, lore, general);
    }

    void Rotigus::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Rotigus::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    Rotigus::ComputePoints,
                    {
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Rotigus", factoryMethod);
        }
    }

    void Rotigus::onWounded() {
        NurgleBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_gnarlrod.setToHit(g_damageTable[damageIndex].m_rodToHit);
        m_fangedMaw.setToWound(g_damageTable[damageIndex].m_mawToWound);
    }

    size_t Rotigus::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Rotigus::onCharged() {
        // Mountain of Loathsome Flesh
        if (m_charged) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto ip : units) {
                int roll = Dice::RollD6();
                if (roll >= 4) {
                    ip->applyDamage({Dice::RollD3(), 0}, this);
                }
            }
        }
        NurgleBase::onCharged();
    }

    Wounds Rotigus::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto unsavedWounds = NurgleBase::applyWoundSave(wounds, attackingUnit);

        // Blubber and Bile
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(unsavedWounds.normal, woundSaves);
        Dice::RollD6(unsavedWounds.mortal, mortalSaves);

        Wounds totalWounds = unsavedWounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        // On each 6+, the attacking unit takes a mortal wound.
        auto numReturnedMortals = mortalSaves.rollsGE(6) + woundSaves.rollsGE(6);
        if (numReturnedMortals > 0)
            attackingUnit->applyDamage({0, numReturnedMortals}, this);

        return totalWounds.clamp();
    }

    void Rotigus::onStartHero(PlayerId player) {
        NurgleBase::onStartHero(player);

        if (owningPlayer() == player) {
            // Corpulent Mass
            heal(Dice::RollD3());

            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);

            // Streams of Brackish Filth
            for (auto unit : units) {
                auto roll = Dice::RollD6();
                if (unit->fly()) {
                    if (roll >= 6) unit->applyDamage({0, Dice::RollD3()}, this);
                } else if (roll >= 4) unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    int Rotigus::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle

