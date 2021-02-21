/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/NomadPrince.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool NomadPrince::s_registered = false;

    Unit *NomadPrince::Create(const ParameterList &parameters) {
        auto unit = new NomadPrince();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

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
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Nomad Prince", factoryMethod);
        }
    }

    NomadPrince::NomadPrince() :
            CitizenOfSigmar("Nomad Prince", 6, g_wounds, 8, 3, false),
            m_spear(Weapon::Type::Melee, "Starlight Spear", 2, 4, 3, 3, -1, 2) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, HERO, NOMAD_PRINCE};
        m_weapons = {&m_spear};
        m_battleFieldRole = Leader;
    }

    bool NomadPrince::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_spear);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int NomadPrince::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar