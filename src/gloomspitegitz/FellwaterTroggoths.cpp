/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/FellwaterTroggoths.h>
#include <Board.h>
#include <Roster.h>
#include <UnitFactory.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    bool FellwaterTroggoths::s_registered = false;

    FellwaterTroggoths::FellwaterTroggoths() :
            GloomspiteGitzBase("Fellwater Troggoths", 6, WOUNDS, 5, 5, false),
            m_noxiousVomit(Weapon::Type::Missile, "Noxious Vomit", 6, 1, 2, 3, -2, RAND_D3),
            m_spikedClub(Weapon::Type::Melee, "Spiked Club", 2, 4, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, FELLWATER};
        m_weapons = {&m_noxiousVomit, &m_spikedClub};
    }

    bool FellwaterTroggoths::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_noxiousVomit);
            model->addMeleeWeapon(&m_spikedClub);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *FellwaterTroggoths::Create(const ParameterList &parameters) {
        auto unit = new FellwaterTroggoths();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void FellwaterTroggoths::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FellwaterTroggoths::Create,
                    nullptr,
                    nullptr,
                    FellwaterTroggoths::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Fellwater Troggoths", factoryMethod);
        }
    }

    void FellwaterTroggoths::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < WOUNDS && remainingWounds() > 0) {
                // Regeneration - heal D3
                // Troggoth Renewal
                if (Dice::rollD6() >= 4 || (inLightOfTheBadMoon() && (Dice::rollD6() >= 4))) {
                    int woundsHealed = Dice::rollD3();
                    if (inLightOfTheBadMoon())
                        woundsHealed *= 2;

                    for (auto &m : m_models) {
                        if (!m->slain() || !m->fled()) {
                            if (m->woundsRemaining() < wounds()) {
                                int numToHeal = std::min(woundsHealed, WOUNDS - m->woundsRemaining());
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

    int FellwaterTroggoths::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace GloomspiteGitz

