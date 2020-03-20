/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "slavestodarkness/SpireTyrants.h"


namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 9;
    static const int MAX_UNIT_SIZE = 36;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool SpireTyrants::s_registered = false;

    Unit *SpireTyrants::Create(const ParameterList &parameters) {
        auto unit = new SpireTyrants();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int SpireTyrants::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void SpireTyrants::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpireTyrants::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    SpireTyrants::ComputePoints,
                    {
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Spire Tyrants", factoryMethod);
        }
    }

    SpireTyrants::SpireTyrants() :
            SlavesToDarknessBase("Spire Tyrants", 6, WOUNDS, 5, 5, false),
            m_gladiatorialWeapons(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 1, 4, 4, 0, 1),
            m_gladiatorialWeaponsChampion(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 3, 4, 4, 0, 1),
            m_gladiatorialWeaponsHeadclaimer(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 1, 4, 4, 0, 2),
            m_gladiatorialWeaponsDestroyer(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 3, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, SPIRE_TYRANTS};
    }

    bool SpireTyrants::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto champion = new Model(BASESIZE, wounds());
        champion->addMeleeWeapon(&m_gladiatorialWeaponsChampion);
        champion->setName("Pit Champion");
        addModel(champion);

        auto headclaimer = new Model(BASESIZE, wounds());
        headclaimer->addMeleeWeapon(&m_gladiatorialWeaponsHeadclaimer);
        headclaimer->setName("Headclaimer");
        addModel(headclaimer);

        auto destroyer = new Model(BASESIZE, wounds());
        destroyer->addMeleeWeapon(&m_gladiatorialWeaponsDestroyer);
        destroyer->setName("Bestigor Destroyer");
        addModel(destroyer);

        for (auto i = 3; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_gladiatorialWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int SpireTyrants::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Pit Fighters
        if (m_charged) mod++;

        return mod;
    }

}//namespace SlavesToDarkness
