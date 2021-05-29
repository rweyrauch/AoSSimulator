/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Jabberslythe.h>
#include <UnitFactory.h>
#include <Board.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 165;

    bool Jabberslythe::s_registered = false;

    Jabberslythe::Jabberslythe(Greatfray fray) :
            BeastsOfChaosBase("Jabberslythe", 12, g_wounds, 6, 4, true, g_pointsPerUnit),
            m_slytheyTongue(Weapon::Type::Missile, "Slythey Tongue", 9, 3, 3, 3, -1, 1),
            m_vorpalClaws(Weapon::Type::Melee, "Vorpal Claws", 1, 6, 3, 3, -2, 1),
            m_spikedTail(Weapon::Type::Melee, "Spiked Tail", 3, 1, 4, 2, -2, RAND_D3) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, JABBERSLYTHE};
        m_weapons = {&m_slytheyTongue, &m_vorpalClaws, &m_spikedTail};
        m_battleFieldRole = Role::Behemoth;

        setGreatfray(fray);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_slytheyTongue);
        model->addMeleeWeapon(&m_vorpalClaws);
        model->addMeleeWeapon(&m_spikedTail);
        addModel(model);
    }

    Unit *Jabberslythe::Create(const ParameterList &parameters) {
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        return new Jabberslythe(fray);
    }

    void Jabberslythe::Init() {
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

            s_registered = UnitFactory::Register("Jabberslythe", factoryMethod);
        }
    }

    int Jabberslythe::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds Jabberslythe::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto totalWounds = BeastsOfChaosBase::computeReturnedDamage(weapon, saveRoll);

        // Spurting Bile Blood
        if (Dice::RollD6() >= 4) {
            totalWounds.mortal++;
        }

        return totalWounds;
    }

} // namespace BeastsOfChaos
