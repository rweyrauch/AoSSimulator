/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Doomwheel.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 150;

    bool Doomwheel::s_registered = false;

    Doomwheel::Doomwheel() :
            Skaventide("Doomwheel", RAND_4D6, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_warpBolts(Weapon::Type::Missile, "Warp Bolts", 13, 0, 3, 3, -1, RAND_D3),
            m_grindingWheel(Weapon::Type::Melee, "Grinding Wheel", 1, RAND_D6, 3, 3, -1, 1),
            m_teethAndKnives(Weapon::Type::Melee, "Teeth and Knives", 1, 6, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WAR_MACHINE, DOOMWHEEL};
        m_weapons = {&m_warpBolts, &m_grindingWheel, &m_teethAndKnives};
        m_battleFieldRole = Role::Behemoth;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_warpBolts);
        model->addMeleeWeapon(&m_grindingWheel);
        model->addMeleeWeapon(&m_teethAndKnives);
        addModel(model);
    }

    Unit *Doomwheel::Create(const ParameterList &parameters) {
        return new Doomwheel();
    }

    void Doomwheel::Init() {
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
            s_registered = UnitFactory::Register("Doomwheel", factoryMethod);
        }
    }

    int Doomwheel::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // More-more Warplead!
        if (weapon->name() == m_warpBolts.name()) {
            // Decide to overcharge
            if (moreMore()) {
                auto roll1 = Dice::RollD6();
                auto roll2 = Dice::RollD6();
                if (roll1 == roll2) {
                    m_moreMoreFailed = true;
                }
                return (roll1 + roll2);
            }

            m_moreMoreFailed = false;
            return RAND_D6;
        }

        return Skaventide::extraAttacks(attackingModel, weapon, target);
    }

    void Doomwheel::onRestore() {
        Skaventide::onRestore();

        m_moreMoreFailed = false;
    }

    void Doomwheel::onEndShooting(PlayerId player) {
        Skaventide::onEndShooting(player);

        if (m_moreMoreFailed) {
            Wounds overchargeWounds = {0, Dice::Roll2D6()};
            applyDamage(overchargeWounds, this);
            m_moreMoreFailed = false;
        }
    }

    int Doomwheel::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace Skaven
