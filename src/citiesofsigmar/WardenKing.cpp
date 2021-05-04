/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/WardenKing.h>
#include <UnitFactory.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool WardenKing::s_registered = false;

    WardenKing::WardenKing(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Warden King", 4, g_wounds, 8, 3, false, g_pointsPerUnit),
            m_runeWeapon(Weapon::Type::Melee, "Rune Hammer", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, WARDEN_KING};
        m_weapons = {&m_runeWeapon};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);
        setNarcotic(narcotic);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_runeWeapon);
        addModel(model);
    }

    Unit *WardenKing::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new WardenKing(city, drug, trait, artefact, general);
    }

    void WardenKing::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WardenKing::Create,
                    CitizenOfSigmar::ValueToString,
                    CitizenOfSigmar::EnumStringToInt,
                    WardenKing::ComputePoints,
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
            s_registered = UnitFactory::Register("Warden King", factoryMethod);
        }
    }

    int WardenKing::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar