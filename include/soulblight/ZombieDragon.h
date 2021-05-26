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

    class ZombieDragon : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit ZombieDragon(CursedBloodline bloodline, Mutation mutation);

        ~ZombieDragon() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_pestilentialBreath,
                m_snappingMaw,
                m_swordlikeClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pestilential Breath              Yes
//

} // namespace Soulblight
