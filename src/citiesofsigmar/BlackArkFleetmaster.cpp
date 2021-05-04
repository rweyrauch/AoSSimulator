/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/BlackArkFleetmaster.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 60;

    bool BlackArkFleetmaster::s_registered = false;

    Unit *BlackArkFleetmaster::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new BlackArkFleetmaster(city, drug, trait, artefact, general);
    }

    std::string BlackArkFleetmaster::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int BlackArkFleetmaster::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void BlackArkFleetmaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BlackArkFleetmaster::Create,
                    BlackArkFleetmaster::ValueToString,
                    BlackArkFleetmaster::EnumStringToInt,
                    BlackArkFleetmaster::ComputePoints,
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
            s_registered = UnitFactory::Register("Black Ark Fleetmaster", factoryMethod);
        }
    }

    BlackArkFleetmaster::BlackArkFleetmaster(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Black Ark Fleetmaster", 6, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_cutlass(Weapon::Type::Melee, "Black Ark Cutlass", 1, 3, 3, 4, 0, 1),
            m_murderHook(Weapon::Type::Melee, "Murder Hook", 1, 2, 4, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, HERO, BLACK_ARK_FLEETMASTER};
        m_weapons = {&m_cutlass, &m_murderHook};
        m_battleFieldRole = Role::Leader;

        setNarcotic(narcotic);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_cutlass);
        model->addMeleeWeapon(&m_murderHook);
        addModel(model);
    }

    int BlackArkFleetmaster::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Murderous Swashbuckler
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_cutlass.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int BlackArkFleetmaster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

}//namespace CitiesOfSigmar