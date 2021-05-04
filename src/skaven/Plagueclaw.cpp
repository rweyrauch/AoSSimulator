/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Plagueclaw.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 150;

    bool Plagueclaw::s_registered = false;

    Unit *Plagueclaw::Create(const ParameterList &parameters) {
        return new Plagueclaw();
    }

    void Plagueclaw::Init() {
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

            s_registered = UnitFactory::Register("Plagueclaw", factoryMethod);
        }
    }

    Plagueclaw::Plagueclaw() :
            Skaventide("Plagueclaw", 3, g_wounds, 4, 5, false, g_pointsPerUnit),
            m_catapult(Weapon::Type::Missile, "Plagueclaw Catapult", 31, 1, 3, 3, -2, RAND_D6),
            m_knives(Weapon::Type::Melee, "Rusty Knives", 1, RAND_D6, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, WAR_MACHINE, PLAGUECLAW};
        m_weapons = {&m_catapult, &m_knives};
        m_battleFieldRole = Role::Artillery;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_catapult);
        model->addMeleeWeapon(&m_knives);
        addModel(model);
    }

    int Plagueclaw::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Skaventide::toHitModifier(weapon, target);

        // Barrage of Disease
        if (target->remainingModels() >= 10) mod++;

        return mod;
    }

    Wounds Plagueclaw::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto wounds = Skaventide::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);

        // Barrage of Disease
        if (target->remainingModels() >= 10) {
            wounds.normal += Dice::Roll2D6();
        }
        return wounds;
    }

    int Plagueclaw::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
