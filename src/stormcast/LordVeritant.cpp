/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordVeritant.h>
#include <iostream>
#include <stormcast/StormcastPrayers.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 110;

    bool LordVeritant::s_registered = false;

    LordVeritant::LordVeritant(Stormhost stormhost, PrayersOfTheStormhost prayer, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Lord-Veritant", 5, g_wounds, 9, 3, false),
            m_judgementBlade(Weapon::Type::Melee, "Judgement Blade", 1, 4, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, PRIEST, LORD_VERITANT};
        m_weapons = {&m_judgementBlade};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_totalPrayers = 2;

        // Lantern of Abjuration
        m_totalUnbinds = 1;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_judgementBlade);
        addModel(model);

        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateSanction(this)));
        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreatePrayerOfTheStormhost(prayer, this)));

        m_points = g_pointsPerUnit;
    }

    Unit *LordVeritant::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto prayer = (PrayersOfTheStormhost) GetEnumParam("Prayers of the Stormhost", parameters,
                                                           g_prayersOfTheStormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsLights[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordVeritant(stormhost, prayer, trait, artefact, general);
    }

    void LordVeritant::Init() {
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
                            EnumParameter("Artefact", g_artefactsLights[0], g_artefactsLights),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Veritant", factoryMethod);
        }
    }

    std::string LordVeritant::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Prayers of the Stormhost") {
            return ToString((PrayersOfTheStormhost) parameter.intValue);
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int LordVeritant::EnumStringToInt(const std::string &enumString) {
        PrayersOfTheStormhost prayer;
        if (FromString(enumString, prayer)) {
            return (int) prayer;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    int LordVeritant::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals