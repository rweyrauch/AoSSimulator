/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/GardusSteelSoul.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 110;

    bool GardusSteelSoul::s_registered = false;

    GardusSteelSoul::GardusSteelSoul(bool isGeneral) :
            StormcastEternal(Stormhost::Hallowed_Knights, "Gardus Steel Soul", 6, g_wounds, 10, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HALLOWED_KNIGHTS, STEEL_SOULS, HERO, LORD_CELESTANT, GARDUS_STEEL_SOUL};
        m_weapons = {&m_runeblade, &m_warhammer};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_runeblade);
        model->addMeleeWeapon(&m_warhammer);
        addModel(model);

        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateFuriousRetribution(this)));

        m_points = g_pointsPerUnit;
    }

    Unit *GardusSteelSoul::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new GardusSteelSoul(general);
    }

    void GardusSteelSoul::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                             BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Gardus Steel Soul", factoryMethod);
        }
    }

    void GardusSteelSoul::onStartShooting(PlayerId player) {
        // Start of my shooting phase.
        if (player == owningPlayer()) {
            // TODO: select any target(s) within 16" rather than the nearest
            if (m_shootingTarget) {
                auto dist = distanceTo(m_shootingTarget);
                if (dist <= 16) {
                    // Sigmarite Warcloak
                    int numStrikes = Dice::RollD6();
                    Dice::RollResult rolls;
                    Dice::RollD6(numStrikes, rolls);
                    int mortalWounds = rolls.rollsGE(4);
                    m_shootingTarget->applyDamage({0, mortalWounds}, this);
                }
            }
        }
        StormcastEternal::onStartShooting(player);
    }

    int GardusSteelSoul::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds GardusSteelSoul::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        Dice::RollResult rolls;
        // Aura of Purity
        Dice::RollD6(wounds.mortal, rolls);
        return StormcastEternal::applyWoundSave({wounds.normal, wounds.mortal - rolls.rollsGE(5), wounds.source}, attackingUnit);
    }

} // namespace StormcastEternals