/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Razorgors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 50;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 6;

    bool Razorgors::s_registered = false;

    Razorgors::Razorgors() :
            BeastsOfChaosBase("Razorgors", 10, WOUNDS, 6, 5, false),
            m_tusksAndHooves(Weapon::Type::Melee, "Large Tusks and Hooves", 1, 4, 4, 3, -1, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, RAZORGORS};
        m_weapons = {&m_tusksAndHooves};
    }

    bool Razorgors::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_tusksAndHooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Razorgors::Create(const ParameterList &parameters) {
        auto unit = new Razorgors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Razorgors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None,
                             BeastsOfChaosBase::Gavespawn, 1},
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Razorgors", factoryMethod);
        }
    }

    Wounds Razorgors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        Wounds damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Uncontrollable Stampede
        if (m_charged && (hitRoll == 6)) {
            damage.mortal += 1;
        }
        return damage;
    }

    int Razorgors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace BeastsOfChaos