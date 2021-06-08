/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/DankholdTroggoths.h>
#include <UnitFactory.h>
#include <Board.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 60;
    static const int g_wounds = 10;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 200;
    static const int g_pointsMaxUnitSize = 600;

    bool DankholdTroggoths::s_registered = false;

    DankholdTroggoths::DankholdTroggoths(Allegiance allegiance, int numModels, int points) :
            GloomspiteGitzBase(allegiance, "Dankhold Troggoths", 6, g_wounds, 6, 4, false, points),
            m_boulderClub(Weapon::Type::Melee, "Boulder Club", 2, 3, 3, 3, -2, RAND_D6) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD};
        m_weapons = {&m_boulderClub};

        s_globalBraveryMod.connect(this, &DankholdTroggoths::reassuringPresence, &m_connection);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_boulderClub);
            addModel(model);
        }
    }

    DankholdTroggoths::~DankholdTroggoths() {
        m_connection.disconnect();
    }

    Unit *DankholdTroggoths::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new DankholdTroggoths(allegiance, numModels, ComputePoints(parameters));
    }

    void DankholdTroggoths::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };

            s_registered = UnitFactory::Register("Dankhold Troggoths", factoryMethod);
        }
    }

    void DankholdTroggoths::onStartHero(PlayerId player) {
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

    void DankholdTroggoths::onStartCombat(PlayerId player) {
        if (m_meleeTarget) {
            // Crushing Grip
            int roll = Dice::RollD6();
            if (roll >= m_meleeTarget->wounds()) {
                m_meleeTarget->slay(1);
            }
        }

        // Squiggly-beast Followers
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= ip->remainingModels()) {
                ip->applyDamage({0, 1}, this);
            }
        }
        GloomspiteGitzBase::onStartCombat(player);
    }

    int DankholdTroggoths::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int DankholdTroggoths::reassuringPresence(const Unit *unit) {
        // Reassuring Presence
        if (unit->hasKeyword(GLOOMSPITE_GITZ) && (unit->owningPlayer() == owningPlayer()) &&
            (distanceTo(unit) <= 18.0)) {
            return 1;
        }

        return 0;
    }

    Wounds DankholdTroggoths::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Magical Resistance
        if (wounds.source == Wounds::Source::Spell) {
            if (Dice::RollD6() >= 4) {
                return {0, 0, Wounds::Source::Spell};
            }
        }
        return GloomspiteGitzBase::applyWoundSave(wounds, attackingUnit);
    }

} // namespace GloomspiteGitz
