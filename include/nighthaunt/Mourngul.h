/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class Mourngul : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Mourngul();

        ~Mourngul() override = default;

    protected:

        void onWounded() override;

        size_t getDamageTableIndex() const;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onRestore() override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_clawsAndFangs{Weapon::Type::Melee, "Nightmarish Claws and Fangs", 2, 8, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// Devourer of Flesh and Souls      Yes
// Ghastly Apparition               Yes


} // namespace Nighthaunt

