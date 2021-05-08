/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/RatlingGun.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 3;
    static const int g_pointsPerUnit = 60;

    bool RatlingGun::s_registered = false;

    RatlingGun::RatlingGun() :
            Skaventide("Ratling Gun", 6, g_wounds, 4, 6, false, g_pointsPerUnit),
            m_ratlingGun(Weapon::Type::Missile, "Ratling Gun", 12, 0, 4, 4, -1, 1),
            m_rustyKnives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, RATLING_GUN};
        m_weapons = {&m_ratlingGun, &m_rustyKnives};

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_ratlingGun);
        model->addMeleeWeapon(&m_rustyKnives);
        addModel(model);
    }

    Unit *RatlingGun::Create(const ParameterList &parameters) {
        return new RatlingGun();
    }

    void RatlingGun::Init() {
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
            s_registered = UnitFactory::Register("Ratling Gun", factoryMethod);
        }
    }

    int RatlingGun::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // More-more Warplead!
        if (weapon->name() == m_ratlingGun.name()) {
            // Decide to overcharge
            if (moreMore()) {
                auto roll1 = Dice::RollD6();
                auto roll2 = Dice::RollD6();
                if (roll1 == roll2) {
                    m_moreMoreFailed = true;
                }
                return (roll1 + roll2) * 2;
            }

            m_moreMoreFailed = false;
            return RAND_2D6;
        }

        return Skaventide::extraAttacks(attackingModel, weapon, target);
    }

    void RatlingGun::onRestore() {
        Skaventide::onRestore();

        m_moreMoreFailed = false;
    }

    void RatlingGun::onEndShooting(PlayerId player) {
        Skaventide::onEndCombat(player);

        if (m_moreMoreFailed) {
            slay(1);
            m_moreMoreFailed = false;
        }
    }

    int RatlingGun::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Skaven
