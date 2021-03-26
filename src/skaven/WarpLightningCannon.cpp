/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarpLightningCannon.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 180;

    bool WarpLightningCannon::s_registered = false;

    WarpLightningCannon::WarpLightningCannon() :
            Skaventide("Warp Lightning Cannon", 3, g_wounds, 4, 4, false),
            m_warpLightningBlast(Weapon::Type::Missile, "Warp Lightning Blast", 24, 0, 0, 0, 0, 0),
            m_teethAndKnives(Weapon::Type::Melee, "Teeth and Knives", 1, RAND_D6, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WAR_MACHINE, WARP_LIGHTNING_CANNON};
        m_weapons = {&m_warpLightningBlast, &m_teethAndKnives};
        m_battleFieldRole = Role::Artillery;
    }

    bool WarpLightningCannon::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_warpLightningBlast);
        model->addMeleeWeapon(&m_teethAndKnives);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *WarpLightningCannon::Create(const ParameterList &parameters) {
        auto unit = new WarpLightningCannon();

        unit->configure();
        return unit;
    }

    void WarpLightningCannon::Init() {
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
            s_registered = UnitFactory::Register("Warp Lightning Cannon", factoryMethod);
        }
    }

    int WarpLightningCannon::generateMortalWounds(const Unit *unit) {
        auto mortalWounds = Skaventide::generateMortalWounds(unit);

        if (m_shootingTarget) {
            bool moreMoreWarpLightning = ((Dice::RollD6() >= 2) || remainingWounds() <= 2);

            // Warp Lightning Blast
            if (distanceTo(m_shootingTarget) <= (double) m_warpLightningBlast.range()) {
                int power = Dice::RollD6();
                Dice::RollResult rollResult;
                Dice::RollD6(6, rollResult);
                mortalWounds += rollResult.rollsGE(power);

                // More-more Warp Lightning!
                if (moreMoreWarpLightning) {
                    bool foundEngineer = false;
                    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
                    for (auto ip : units) {
                        if (ip->hasKeyword(WARLOCK) && ip->hasKeyword(ENGINEER)) {
                            foundEngineer = true;
                            break;
                        }
                    }

                    if (foundEngineer) {
                        Dice::RollResult rollResultMore;
                        Dice::RollD6(6, rollResultMore);
                        mortalWounds += rollResultMore.rollsGE(power);

                        int mortalWoundsSelf = 0;
                        int numUnmodifiedOnes = rollResult.numUnmodified1s() + rollResultMore.numUnmodified1s();
                        for (auto i = 0; i < numUnmodifiedOnes; i++) {
                            mortalWoundsSelf += Dice::RollD3();
                        }
                        applyDamage({0, mortalWoundsSelf}, this);
                    }
                }
            }
        }

        return mortalWounds;
    }

    int WarpLightningCannon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Skaven
