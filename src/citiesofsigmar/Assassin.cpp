/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Assassin.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Assassin::s_registered = false;

    Unit *Assassin::Create(const ParameterList &parameters) {
        auto unit = new Assassin();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto drug = (Narcotic)GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        unit->setNarcotic(drug);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Assassin::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Assassin::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Assassin::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Assassin::Create,
                    Assassin::ValueToString,
                    Assassin::EnumStringToInt,
                    Assassin::ComputePoints,
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
            s_registered = UnitFactory::Register("Assassin", factoryMethod);
        }
    }

    Assassin::Assassin() :
            CitizenOfSigmar("Assassin", 6, g_wounds, 7, 5, false),
            m_blades(Weapon::Type::Melee, "Poison-coated Blades", 1, 6, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SHADOWBLADES, HERO, ASSASSIN};
        m_weapons = {&m_blades};
        m_battleFieldRole = Leader;
    }

    bool Assassin::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds Assassin::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deathshead Poison
        if ((woundRoll == 6) && (weapon->name() == m_blades.name())) {
            return {0, Dice::RollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Assassin::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar