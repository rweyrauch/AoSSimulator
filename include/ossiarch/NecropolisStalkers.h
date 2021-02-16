/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class NecropolisStalkers : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        NecropolisStalkers();

        ~NecropolisStalkers() override = default;

        bool configure(int numModels, int numFalchions);

    protected:

        void onStartCombat(PlayerId player) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        enum Aspect {
            Blade_Strike,
            Blade_Parry,
            Destroyer,
            Precision
        };

        Aspect m_activeAspect = Blade_Strike;

        Weapon m_falchions,
                m_blades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Quadrarch Aspects                Yes (random)
//      Blade-strike Aspect         Yes
//      Blade-parry Aspect          Yes
//      Destroyer Aspect            Yes
//      Precision Aspect            Yes
// Hunt and Kill                    TODO

} // namespace OssiarchBonereapers

