/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/RockgutTroggoths.h>
#include <Board.h>
#include <Roster.h>
#include <UnitFactory.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 140 * 4;

    bool RockgutTroggoths::s_registered = false;

    RockgutTroggoths::RockgutTroggoths(Allegiance allegiance, int numModels, int points) :
            GloomspiteGitzBase(allegiance, "Rockgut Troggoths", 6, g_wounds, 5, 5, false, points),
            m_massiveStoneMaul(Weapon::Type::Melee, "Massive Stone Maul", 2, 2, 3, 3, -2, 3) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, ROCKGUT};
        m_weapons = {&m_massiveStoneMaul};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_massiveStoneMaul);
            addModel(model);
        }
    }

    Unit *RockgutTroggoths::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new RockgutTroggoths(allegiance, numModels, ComputePoints(parameters));
    }

    void RockgutTroggoths::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    RockgutTroggoths::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    RockgutTroggoths::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Rockgut Troggoths", factoryMethod);
        }
    }

    void RockgutTroggoths::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                // Regeneration - heal D3
                // Troggoth Renewal
                if (Dice::RollD6() >= 4 || (inLightOfTheBadMoon() && (Dice::RollD6() >= 4))) {
                    int woundsHealed = Dice::RollD3();
                    if (inLightOfTheBadMoon())
                        woundsHealed *= 2;

                    for (auto &m : m_models) {
                        if (!m->slain() || !m->fled()) {
                            if (m->woundsRemaining() < wounds()) {
                                int numToHeal = std::min(woundsHealed, g_wounds - m->woundsRemaining());
                                m->applyHealing(numToHeal);
                                woundsHealed -= numToHeal;
                                if (woundsHealed <= 0) { break; }
                            }
                        }
                    }
                }
            }
        }
    }

    Wounds RockgutTroggoths::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Stony Skin
        int numSixes = 0;
        return ignoreWounds(wounds, 5, numSixes);
    }

    void RockgutTroggoths::onStartShooting(PlayerId player) {
        auto board = Board::Instance();
        PlayerId otherPlayer = PlayerId::Red;
        if (player == PlayerId::Red) {
            otherPlayer = PlayerId::Blue;
        }
        auto otherRoster = board->getPlayerRoster(otherPlayer);

        auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
        if (nearestUnit) {
            // Throwin Boulders
            double rangeTo = distanceTo(nearestUnit);
            if (rangeTo < 12) {
                int roll = Dice::RollD6();
                if (roll <= nearestUnit->remainingModels()) {
                    nearestUnit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }

        GloomspiteGitzBase::onStartShooting(player);
    }

    int RockgutTroggoths::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace GloomspiteGitz

