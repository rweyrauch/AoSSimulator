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

    class AuralanWardens : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuralanWardens();

        ~AuralanWardens() override = default;

        bool configure(int numModels);

    protected:

        void onRestore() override;
        void onStartCombat(PlayerId player) override;
        void onStartHero(PlayerId player) override;
        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
        void onCastSpell(const Spell *spell, const Unit *target) override;
        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_championsBlade,
            m_wardensPike;

        bool m_usedMoonfireFlask = false;
        bool m_powerOfHyshActive = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Moonfire Flask                   Yes
// Sunmetal Weapons                 Yes
// Wall of Blades                   Yes
// Power of Hysh                    Yes
//

} // namespace LuminethRealmLords

