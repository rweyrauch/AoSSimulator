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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        NecropolisStalkers(Legion legion, int numModels, int numFalchions, int points);

        NecropolisStalkers() = delete;

        ~NecropolisStalkers() override = default;

    protected:

        void onStartCombat(PlayerId player) override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        enum Aspect {
            Blade_Strike,
            Blade_Parry,
            Destroyer,
            Precision
        };

        Aspect m_activeAspect = Blade_Strike;

        Weapon  m_falchions{Weapon::Type::Melee, "Dread Falchions", 1, 3, 4, 3, -2, 2},
                m_blades{Weapon::Type::Melee, "Spirit Blades", 1, 5, 3, 3, -1, 1};

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

