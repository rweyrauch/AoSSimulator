/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Jabberslythe.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 160;

    bool Jabberslythe::s_registered = false;

    Jabberslythe::Jabberslythe() :
            BeastsOfChaosBase("Jabberslythe", 12, WOUNDS, 6, 5, true),
            m_slytheyTongue(Weapon::Type::Missile, "Slythey Tongue", 9, 1, 3, 3, -1, RAND_D3),
            m_vorpalClaws(Weapon::Type::Melee, "Vorpal Claws", 1, 6, 3, 3, -2, 1),
            m_spikedTail(Weapon::Type::Melee, "Spiked Tail", 3, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, JABBERSLYTHE};
        m_weapons = {&m_slytheyTongue, &m_vorpalClaws, &m_spikedTail};
        m_battleFieldRole = Behemoth;
    }

    bool Jabberslythe::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_slytheyTongue);
        model->addMeleeWeapon(&m_vorpalClaws);
        model->addMeleeWeapon(&m_spikedTail);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Jabberslythe::Create(const ParameterList &parameters) {
        auto unit = new Jabberslythe();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    int Jabberslythe::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    Wounds Jabberslythe::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto totalWounds = Unit::computeReturnedDamage(weapon, saveRoll);

        // Spurting Bile Blood
        if (Dice::rollD6() >= 4) {
            totalWounds.mortal++;
        }

        return totalWounds;
    }

} // namespace BeastsOfChaos
