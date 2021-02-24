/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skarbrand.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 100;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 380;

    struct TableEntry {
        int m_roarOfTotalRage;
        int m_slaughterAttacks;
        int m_totalCarnage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {1, 5, 5},
                    {2, 6, 4},
                    {3, 7, 3},
                    {4, 8, 2},
                    {5, 9, 1}
            };

    bool Skarbrand::s_registered = false;

    Skarbrand::Skarbrand() :
            KhorneBase("Skarbrand", 8, g_wounds, 10, 4, true),
            m_slaughter(Weapon::Type::Melee, "Slaughter", 2, 5, 4, 3, -2, 3),
            m_carnage(Weapon::Type::Melee, "Carnage", 2, 1, 4, 0, 0, 0) {
        m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, SKARBRAND};
        m_weapons = {&m_slaughter, &m_carnage};
        m_battleFieldRole = Leader_Behemoth;
    }

    bool Skarbrand::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_slaughter);
        // Do not add Carnage or Roar of Total Rage, their attacks are special.
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Skarbrand::Create(const ParameterList &parameters) {
        auto unit = new Skarbrand();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Skarbrand::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Skarbrand::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Skarbrand::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skarbrand", factoryMethod);
        }
    }

    void Skarbrand::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_slaughter.setAttacks(g_damageTable[damageIndex].m_slaughterAttacks);

        KhorneBase::onWounded();
    }

    int Skarbrand::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Rerolls Skarbrand::chargeRerolls() const {
        // Inescapable Wrath
        return Reroll_Failed;
    }

    int Skarbrand::generateMortalWounds(const Unit *unit) {
        // Total Carnage
        if (distanceTo(unit) <= (double) m_carnage.range()) {
            int index = getDamageTableIndex();
            if (!m_attackedInPreviousRound)
                index = g_numTableEntries - 1;

            m_attackedInPreviousRound = true;

            int roll = Dice::RollD6();
            if (roll >= g_damageTable[index].m_totalCarnage) {
                int mortals = 8;
                if (roll == 6)
                    mortals = 16;

                PLOG_INFO.printf("Skarbrand Total Carnage inflicts %d mortal wounds on to %s",
                       mortals, unit->name().c_str());

                return mortals;
            }
        }

        return KhorneBase::generateMortalWounds(unit);
    }

    void Skarbrand::onStartShooting(PlayerId player) {
        // Roar of Total Rage
        if (m_shootingTarget) {
            if (distanceTo(m_shootingTarget) <= 8.0) {
                Dice::RollResult results;
                Dice::RollD6(g_damageTable[getDamageTableIndex()].m_roarOfTotalRage, results);
                if (results.rollsGE(4)) {
                    Wounds wounds = {0, results.rollsGE(4)};
                    m_shootingTarget->applyDamage(wounds, this);

                    PLOG_INFO.printf("Skarbrand Roar of Total Rage inflicts %d mortal wounds on to %s",
                           wounds.mortal, m_shootingTarget->name().c_str());
                }
                m_attackedInPreviousRound = true;
            }
        }
        KhorneBase::onStartShooting(player);
    }

    int Skarbrand::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne
