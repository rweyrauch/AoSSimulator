/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class TheLightOfEltharion : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheLightOfEltharion();

        ~TheLightOfEltharion() override = default;

    protected:

        void configure();

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
        void onStartShooting(PlayerId player) override;
        int targetSaveModifier(const Weapon *weapon, const Unit *attacker) const override;
        Wounds targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll, int woundRoll) const override;
        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
        void onStartCombat(PlayerId player) override;

    private:

        Weapon m_fangsword,
            m_blade;

        Unit* m_celennariBladeTarget = nullptr;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Celenari Blade                   Yes
// Fangsword of Eltharion           Yes
// Searing Darts of Light           Yes
// Spirit Armour                    Yes
// Supreme Swordmaster              Yes/TODO hit modifier
// Unflinching Valour               TODO
//

} // namespace LuminethRealmLords

