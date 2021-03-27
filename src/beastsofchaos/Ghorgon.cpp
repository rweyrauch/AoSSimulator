/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Ghorgon.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 160;

    bool Ghorgon::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_bladesAttacks;
        int m_greatMawToWound;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 5, 2},
                    {6, 4, 3},
                    {5, 3, 3},
                    {4, 2, 4},
                    {3, 1, 4}
            };

    Ghorgon::Ghorgon() :
            BeastsOfChaosBase("Ghorgon", 8, g_wounds, 7, 5, false),
            m_butcheringBlades(Weapon::Type::Melee, "Butchering Blades", 2, 5, 3, 3, -1, 3),
            m_hugeSlaveringMaw(Weapon::Type::Melee, "Huge Slavering Maw", 1, 1, 4, 2, -1, RAND_D6) {
        m_keywords = {CHAOS, BULLGOR, BEASTS_OF_CHAOS, WARHERD, MONSTER, GHORGON};
        m_weapons = {&m_butcheringBlades, &m_hugeSlaveringMaw};
        m_battleFieldRole = Role::Behemoth;
    }

    bool Ghorgon::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_butcheringBlades);
        model->addMeleeWeapon(&m_hugeSlaveringMaw);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void Ghorgon::onRestore() {
        // Reset table-driven attributes
        onWounded();
    }

    Unit *Ghorgon::Create(const ParameterList &parameters) {
        auto unit = new Ghorgon();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        unit->configure();
        return unit;
    }

    void Ghorgon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Ghorgon", factoryMethod);
        }
    }

    void Ghorgon::onWounded() {
        const auto damageIndex = getDamageTableIndex();

        m_butcheringBlades.setAttacks(g_damageTable[damageIndex].m_bladesAttacks);
        m_hugeSlaveringMaw.setDamage(g_damageTable[damageIndex].m_greatMawToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    size_t Ghorgon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds Ghorgon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Ravenous Bloodgreed
        if (hitRoll == 6) {
            return {weapon->damage(), Dice::RollD3()};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Ghorgon::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (player != owningPlayer()) { return; }

        // Swallowed Whole
        if (m_meleeTarget) {
            int roll = Dice::RollD6();
            if (roll >= m_meleeTarget->wounds()) {
                m_meleeTarget->slay(1);
            }
        }
    }

    int Ghorgon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
