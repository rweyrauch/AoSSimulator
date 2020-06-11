/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/CelestantPrime.h>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 100; //
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 340;

    bool CelestantPrime::s_registered = false;

    Unit *CelestantPrime::Create(const ParameterList &parameters) {
        auto unit = new CelestantPrime();

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void CelestantPrime::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Celestant Prime", factoryMethod);
        }
    }

    std::string CelestantPrime::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int CelestantPrime::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    CelestantPrime::CelestantPrime() :
            StormcastEternal("Celestant Prime", 12, WOUNDS, 10, 3, true),
            m_ghalMaraz(Weapon::Type::Melee, "Ghal Maraz, the Hammer of Sigmar", 2, 3, 3, 2, -3, 3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, CELESTANT_PRIME};
        m_weapons = {&m_ghalMaraz};

        s_globalBraveryMod.connect(this, &CelestantPrime::bearerOfTheWarhammer, &m_connection);
    }

    CelestantPrime::~CelestantPrime() {
        m_connection.disconnect();
    }

    bool CelestantPrime::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_ghalMaraz);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int CelestantPrime::bearerOfTheWarhammer(const Unit *target) {
        // Bearer of the Warhammer
        if (target->hasKeyword(ORDER) && (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 18.0)) {
            return 1;
        }

        return 0;
    }

    int CelestantPrime::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }


} // namespace StormcastEternals