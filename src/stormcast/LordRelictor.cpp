/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordRelictor.h>
#include <iostream>
#include <stormcast/StormcastPrayers.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool LordRelictor::s_registered = false;

    LordRelictor::LordRelictor(Stormhost stormhost, PrayersOfTheStormhost prayer, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Lord-Relictor", 5, g_wounds, 9, 3, false, g_pointsPerUnit),
            m_relicHammer(Weapon::Type::Melee, "Relic Hammer", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, PRIEST, LORD_RELICTOR};
        m_weapons = {&m_relicHammer};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_totalPrayers = 2;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_relicHammer);
        addModel(model);

        m_points = g_pointsPerUnit;

        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateLightningStorm(this)));
        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateHealingStorm(this)));
        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreatePrayerOfTheStormhost(prayer, this)));
    }

    Unit *LordRelictor::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto prayer = (PrayersOfTheStormhost) GetEnumParam("Prayers of the Stormhost", parameters,
                                                           g_prayersOfTheStormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordRelictor(stormhost, prayer, trait, artefact, general);
    }

    void LordRelictor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Prayers of the Stormhost", g_prayersOfTheStormhost[0],
                                          g_prayersOfTheStormhost),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Relictor", factoryMethod);
        }
    }

    std::string LordRelictor::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Prayers of the Stormhost") {
            return ToString((PrayersOfTheStormhost) parameter.intValue);
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int LordRelictor::EnumStringToInt(const std::string &enumString) {
        PrayersOfTheStormhost prayer;
        if (FromString(enumString, prayer)) {
            return (int) prayer;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int LordRelictor::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals