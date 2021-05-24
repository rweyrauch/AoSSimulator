/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Destruction {

    class Kragnos : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit Kragnos(bool isGeneral);

        ~Kragnos() override;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls chargeRerolls() const override;

        void onCharged() override;

        int iconOfDestruction(const Unit *target);

    private:

        lsignal::slot m_iconOfDestruction;

        Weapon  m_mace{Weapon::Type::Melee, "The Dread Mace", 3, 6, 3, 2, -3, 4},
                m_tuskbreaker{Weapon::Type::Melee, "Tuskbreaker", 1, 3, 3, 2, -2, RAND_D3},
                m_hooves{Weapon::Type::Melee, "Hooves of Wrack and Ruin", 1, 6, 3, 2, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bellow of Rage                   TODO
// Destroyer of the Draconith Empire Yes
// Rampaging Destruction            Yes
// Icon of Destruction              Yes
// The Shield Inviolate             TODO
//

} // namespace Destruction
