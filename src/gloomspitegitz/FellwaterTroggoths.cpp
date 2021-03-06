/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/FellwaterTroggoths.h>
#include <Roster.h>
#include <UnitFactory.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 150;
    static const int g_pointsMaxUnitSize = 600;

    bool FellwaterTroggoths::s_registered = false;

    FellwaterTroggoths::FellwaterTroggoths(Allegiance allegiance, int numModels, int points) :
            GloomspiteGitzBase(allegiance, "Fellwater Troggoths", 6, g_wounds, 5, 5, false, points),
            m_noxiousVomit(Weapon::Type::Missile, "Noxious Vomit", 6, 1, 2, 3, -2, RAND_D3),
            m_spikedClub(Weapon::Type::Melee, "Spiked Club", 2, 4, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, FELLWATER};
        m_weapons = {&m_noxiousVomit, &m_spikedClub};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_noxiousVomit);
            model->addMeleeWeapon(&m_spikedClub);
            addModel(model);
        }
    }

    Unit *FellwaterTroggoths::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new FellwaterTroggoths(allegiance, numModels, ComputePoints(parameters));
    }

    void FellwaterTroggoths::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FellwaterTroggoths::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    FellwaterTroggoths::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Fellwater Troggoths", factoryMethod);
        }
    }

    void FellwaterTroggoths::onStartHero(PlayerId player) {
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

    int FellwaterTroggoths::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);

        if (!weapon->isMissile()) {
            // Terrible Stench
            modifier -= 1;
        }
        return modifier;
    }

    int FellwaterTroggoths::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace GloomspiteGitz

