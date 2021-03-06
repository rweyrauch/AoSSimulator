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
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 3;
    static const int g_pointsPerUnit = 70;

    bool WarpfireThrower::s_registered = false;

    WarpfireThrower::WarpfireThrower() :
            Skaventide("Warpfire Thrower", 6, g_wounds, 4, 6, false, g_pointsPerUnit),
            m_warpfireThrower(Weapon::Type::Missile, "Warpfire Thrower", 8, 0, 0, 0, 0, 0),
            m_rustyKnives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, WARPFIRE_THROWER};
        m_weapons = {&m_warpfireThrower, &m_rustyKnives};

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_warpfireThrower);
        model->addMeleeWeapon(&m_rustyKnives);
        addModel(model);
    }

    Unit *WarpfireThrower::Create(const ParameterList &parameters) {
        return new WarpfireThrower();
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
            bool moreMoreWarpfire = ((Dice::RollD6() >= 2) || remainingWounds() <= 1);

            // Warpfire
            if (distanceTo(m_shootingTarget) <= (double) m_warpfireThrower.range()) {
                int numTargetModels = m_shootingTarget->remainingModels();
                Dice::RollResult rollResult;
                Dice::RollD6(numTargetModels, rollResult);
                mortalWounds += rollResult.rollsGE(4);

                // More-more Warpfire!
                if (moreMoreWarpfire) {
                    Dice::RollD6(numTargetModels, rollResult);
                    mortalWounds += rollResult.rollsGE(4);

                    int roll = Dice::RollD6();
                    if (roll <= 2) {
                        // this model is slain
                        slay(1);
                    }
                }
            }
        }

        return mortalWounds;
    }

    int WarpfireThrower::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Skaven
