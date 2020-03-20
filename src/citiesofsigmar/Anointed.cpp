/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Anointed.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool Anointed::s_registered = false;

    Unit *Anointed::Create(const ParameterList &parameters) {
        auto unit = new Anointed();

        auto city = (City) GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Anointed::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Anointed::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Anointed::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Anointed::Create,
                    Anointed::ValueToString,
                    Anointed::EnumStringToInt,
                    Anointed::ComputePoints,
                    {
                            {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal,
                             CitizenOfSigmar::TempestsEye, 1},
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Anointed", factoryMethod);
        }
    }

    Anointed::Anointed() :
            CitizenOfSigmar("Anointed", 6, WOUNDS, 7, 4, false),
            m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, HERO, ANOINTED};
        m_weapons = {&m_halberd};
        // Blessing of the Ur-Phoenix
        m_totalUnbinds = 1;
    }

    bool Anointed::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_halberd);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds Anointed::applyWoundSave(const Wounds &wounds) {
        // Witness to Destiny
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(4);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(4);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int Anointed::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} //namespace CitiesOfSigmar