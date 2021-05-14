/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class MortisEngine : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MortisEngine(Legion legion);

        ~MortisEngine() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartShooting(PlayerId player) override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_wail,
                m_staff,
                m_etherealWeapons;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Wail of the Damned               Yes
// Frightful Touch                  Yes
// The Reliquery                    TODO
// Bound Necromancer                TODO
//

} // namespace Soulblight
