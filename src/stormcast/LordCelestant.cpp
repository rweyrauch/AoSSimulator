/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestant.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool LordCelestant::s_registered = false;

    LordCelestant::LordCelestant() :
            StormcastEternal("Lord-Celestant", 5, WOUNDS, 9, 3, false),
            m_runeblade(Weapon::Type::Melee, "Sigmarite Runeblade", 1, 4, 3, 3, -1, 1),
            m_warhammer(Weapon::Type::Melee, "Warhammer", 1, 2, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, LORD_CELESTANT};
        m_weapons = {&m_runeblade, &m_warhammer};
    }

    bool LordCelestant::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_runeblade);
        model->addMeleeWeapon(&m_warhammer);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LordCelestant::Create(const ParameterList &parameters) {
        auto unit = new LordCelestant();

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, StormcastEternal::None);
        unit->setStormhost(stormhost);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordCelestant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None,
                             StormcastEternal::AstralTemplars, 1},
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Celestant", factoryMethod);
        }
    }

    void LordCelestant::onStartShooting(PlayerId player) {
        // Start of my shooting phase.
        if (player == owningPlayer()) {
            // TODO: select any target(s) within 16" rather than the nearest
            if (m_shootingTarget) {
                auto dist = distanceTo(m_shootingTarget);
                if (dist <= 16) {
                    // Sigmarite Warcloak
                    int numStrikes = Dice::rollD6();
                    Dice::RollResult rolls;
                    Dice::rollD6(numStrikes, rolls);
                    int mortalWounds = rolls.rollsGE(4);
                    m_shootingTarget->applyDamage({0, mortalWounds});
                }
            }
        }
        StormcastEternal::onStartShooting(player);
    }

    int LordCelestant::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals