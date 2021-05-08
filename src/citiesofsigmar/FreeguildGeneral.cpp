/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGeneral.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool FreeguildGeneral::s_registered = false;

    Unit *FreeguildGeneral::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new FreeguildGeneral(city, drug, trait, artefact, general);
    }

    std::string FreeguildGeneral::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildGeneral::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildGeneral::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
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
            s_registered = UnitFactory::Register("Freeguild General", factoryMethod);
        }
    }

    FreeguildGeneral::FreeguildGeneral(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Freeguild General", 5, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_zweihander(Weapon::Type::Melee, "Zweihander", 1, 3, 3, 3, -2, RAND_D3) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, HERO, FREEGUILD_GENERAL};
        m_weapons = {&m_zweihander};
        m_battleFieldRole = Role::Leader;

        s_globalBraveryMod.connect(this, &FreeguildGeneral::inspiringLeader, &m_connection);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);
        setNarcotic(narcotic);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_zweihander);
        addModel(model);
    }

    FreeguildGeneral::~FreeguildGeneral() {
        m_connection.disconnect();
    }

    Wounds FreeguildGeneral::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Swing
        auto damage = CitizenOfSigmar::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        if (hitRoll == 6) damage.mortal++;
        return damage;
    }

    int FreeguildGeneral::inspiringLeader(const Unit *target) {
        // Inspiring Leader
        if (target->hasKeyword(FREEGUILD) && (target->owningPlayer() == owningPlayer()) &&
            (distanceTo(target) <= 18.0)) {
            return 1;
        }

        return 0;
    }

    int FreeguildGeneral::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar