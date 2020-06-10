/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/NomadPrince.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool NomadPrince::s_registered = false;

    Unit *NomadPrince::Create(const ParameterList &parameters) {
        auto unit = new NomadPrince();

        auto city = (City) GetEnumParam("City", parameters, Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string NomadPrince::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int NomadPrince::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void NomadPrince::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    NomadPrince::Create,
                    NomadPrince::ValueToString,
                    NomadPrince::EnumStringToInt,
                    NomadPrince::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Nomad Prince", factoryMethod);
        }
    }

    NomadPrince::NomadPrince() :
            CitizenOfSigmar("Nomad Prince", 6, WOUNDS, 8, 3, false),
            m_spear(Weapon::Type::Melee, "Starlight Spear", 2, 4, 3, 3, -1, 2) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, HERO, NOMAD_PRINCE};
        m_weapons = {&m_spear};
    }

    bool NomadPrince::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_spear);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int NomadPrince::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace CitiesOfSigmar