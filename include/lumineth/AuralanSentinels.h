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

    class AuralanSentinels : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuralanSentinels();

        ~AuralanSentinels() override = default;

        bool configure(int numModels);

    protected:

        void onRestore() override;
        void onStartHero(PlayerId player) override;
        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
        void onCastSpell(const Spell *spell, const Unit *target) override;
        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_bowAimed,
            m_bowLofted,
            m_blade,
            m_dagger;

        bool m_powerOfHyshActive = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Skyhawk Lantern                  TODO
// Sunmetal Weapons                 Yes
// Many-stringed Weapons            Yes
// Power of Hysh                    Yes
//

} // namespace LuminethRealmLords

