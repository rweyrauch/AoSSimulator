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

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool LordVeritant::s_registered = false;

    LordVeritant::LordVeritant() :
            StormcastEternal("Lord-Veritant", 5, WOUNDS, 9, 3, false),
            m_judgementBlade(Weapon::Type::Melee, "Judgement Blade", 1, 4, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, PRIEST, LORD_VERITANT};
        m_weapons = {&m_judgementBlade};

        m_totalPrayers = 2;

        // Lantern of Abjuration
        m_totalUnbinds = 1;
    }

    bool LordVeritant::configure(PrayersOfTheStormhost prayer) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_judgementBlade);
        addModel(model);

        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateSanction(this)));
        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreatePrayerOfTheStormhost(prayer, this)));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LordVeritant::Create(const ParameterList &parameters) {
        auto unit = new LordVeritant();
        auto prayer = (PrayersOfTheStormhost) GetEnumParam("Prayers of the Stormhost", parameters,
                                                           g_prayersOfTheStormhost[0]);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, NoStormhost);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordVeritant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Prayers of the Stormhost", g_prayersOfTheStormhost[0], g_prayersOfTheStormhost),
                            EnumParameter("Stormhost", NoStormhost, g_stormhost)
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
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals