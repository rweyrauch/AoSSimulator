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
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool LordRelictor::s_registered = false;

    LordRelictor::LordRelictor() :
            StormcastEternal("Lord-Relictor", 5, WOUNDS, 9, 3, false),
            m_relicHammer(Weapon::Type::Melee, "Relic Hammer", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, PRIEST, LORD_RELICTOR};
        m_weapons = {&m_relicHammer};

        m_totalPrayers = 2;
    }

    bool LordRelictor::configure(PrayersOfTheStormhost prayer) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_relicHammer);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateLightningStorm(this)));
        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateHealingStorm(this)));
        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreatePrayerOfTheStormhost(prayer, this)));

        return true;
    }

    Unit *LordRelictor::Create(const ParameterList &parameters) {
        auto unit = new LordRelictor();
        auto prayer = (PrayersOfTheStormhost) GetEnumParam("Prayers of the Stormhost", parameters,
                                                           g_prayersOfTheStormhost[0]);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordRelictor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Prayers of the Stormhost", g_prayersOfTheStormhost[0], g_prayersOfTheStormhost),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait)
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

    int LordRelictor::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals