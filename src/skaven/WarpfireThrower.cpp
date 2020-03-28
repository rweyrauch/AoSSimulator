/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarpfireThrower.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 70;

    bool WarpfireThrower::s_registered = false;

    WarpfireThrower::WarpfireThrower() :
            Skaventide("Warpfire Thrower", 6, WOUNDS, 4, 6, false),
            m_warpfireThrower(Weapon::Type::Missile, "Warpfire Thrower", 8, 0, 0, 0, 0, 0),
            m_rustyKnives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, WARPFIRE_THROWER};
        m_weapons = {&m_warpfireThrower, &m_rustyKnives};
    }

    bool WarpfireThrower::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_warpfireThrower);
        model->addMeleeWeapon(&m_rustyKnives);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *WarpfireThrower::Create(const ParameterList &parameters) {
        auto unit = new WarpfireThrower();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WarpfireThrower::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Warpfire Thrower", factoryMethod);
        }
    }

    int WarpfireThrower::generateMortalWounds(const Unit *unit) {
        auto mortalWounds = Skaventide::generateMortalWounds(unit);

        if (m_shootingTarget) {
            bool moreMoreWarpfire = ((Dice::rollD6() >= 2) || remainingWounds() <= 1);

            // Warpfire
            if (distanceTo(m_shootingTarget) <= (float) m_warpfireThrower.range()) {
                int numTargetModels = m_shootingTarget->remainingModels();
                Dice::RollResult rollResult;
                Dice::rollD6(numTargetModels, rollResult);
                mortalWounds += rollResult.rollsGE(4);

                // More-more Warpfire!
                if (moreMoreWarpfire) {
                    Dice::rollD6(numTargetModels, rollResult);
                    mortalWounds += rollResult.rollsGE(4);

                    int roll = Dice::rollD6();
                    if (roll <= 2) {
                        // this model is slain
                        slay(1);
                    }
                }
            }
        }

        return mortalWounds;
    }

    int WarpfireThrower::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Skaven
