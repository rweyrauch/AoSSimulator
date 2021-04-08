/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class WrathOfKhorneBloodthirster : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        WrathOfKhorneBloodthirster() = delete;

        ~WrathOfKhorneBloodthirster() override = default;

    protected:

        WrathOfKhorneBloodthirster(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        size_t getDamageTableIndex() const;

        void onWounded() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int unbindingModifier() const override { return 2; }

    private:

        Weapon m_bloodflail,
                m_mightyAxeOfKhorne,
                m_breath;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hellfire Breath                  Yes
// Relentless Hunter                Yes
// Rune-crown of Khorne             Yes
// Lord of the Blood Hunt           Yes
//

} // namespace Khorne
