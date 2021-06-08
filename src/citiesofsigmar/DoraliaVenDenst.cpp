/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DoraliaVenDenst.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 115;

    bool DoraliaVenDenst::s_registered = false;

    Unit *DoraliaVenDenst::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new DoraliaVenDenst(city, drug, trait, artefact, general);
    }

    void DoraliaVenDenst::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DoraliaVenDenst::Create,
                    CitizenOfSigmar::ValueToString,
                    CitizenOfSigmar::EnumStringToInt,
                    DoraliaVenDenst::ComputePoints,
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
            s_registered = UnitFactory::Register("Doralia ven Denst", factoryMethod);
        }
    }

    DoraliaVenDenst::DoraliaVenDenst(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Doralia ven Denst", 5, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, HERO, WITCH_HUNTER, DORALIA_VEN_DENST};
        m_weapons = {&m_crossbow, &m_sword};
        m_battleFieldRole = Role::Leader;

        setNarcotic(narcotic);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_crossbow);
        model->addMeleeWeapon(&m_sword);
        addModel(model);
    }

    int DoraliaVenDenst::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar