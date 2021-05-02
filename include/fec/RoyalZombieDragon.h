/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class RoyalZombieDragon : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        RoyalZombieDragon();

        ~RoyalZombieDragon() override = default;

    protected:

        void configure();

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

} // namespace FleshEaterCourt
