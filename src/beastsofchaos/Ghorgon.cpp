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
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 160;

    bool Ghorgon::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_bladesAttacks;
        int m_greatMawToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {8, 5, 2},
                    {6, 4, 3},
                    {5, 3, 3},
                    {4, 2, 4},
                    {3, 1, 4}
            };

    Ghorgon::Ghorgon() :
            BeastsOfChaosBase("Ghorgon", 8, WOUNDS, 7, 5, false),
            m_butcheringBlades(Weapon::Type::Melee, "Butchering Blades", 2, 5, 3, 3, -1, 3),
            m_hugeSlaveringMaw(Weapon::Type::Melee, "Huge Slavering Maw", 1, 1, 4, 2, -1, RAND_D6) {
        m_keywords = {CHAOS, BULLGOR, BEASTS_OF_CHAOS, WARHERD, MONSTER, GHORGON};
        m_weapons = {&m_butcheringBlades, &m_hugeSlaveringMaw};
        m_battleFieldRole = Behemoth;
    }

    bool Ghorgon::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_butcheringBlades);
        model->addMeleeWeapon(&m_hugeSlaveringMaw);
        addModel(model);

        m_points = POINTS_PER_UNIT;

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

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
        const int damageIndex = getDamageTableIndex();

        m_butcheringBlades.setAttacks(g_damageTable[damageIndex].m_bladesAttacks);
        m_hugeSlaveringMaw.setDamage(g_damageTable[damageIndex].m_greatMawToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int Ghorgon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds Ghorgon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Ravenous Bloodgreed
        if (hitRoll == 6) {
            return {weapon->damage(), Dice::rollD3()};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Ghorgon::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (player != owningPlayer()) { return; }

        // Swallowed Whole
        if (m_meleeTarget) {
            int roll = Dice::rollD6();
            if (roll >= m_meleeTarget->wounds()) {
                m_meleeTarget->slay(1);
            }
        }
    }

    int Ghorgon::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace BeastsOfChaos
