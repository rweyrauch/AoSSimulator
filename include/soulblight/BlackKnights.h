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

    class BlackKnights : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BlackKnights(CursedBloodline bloodline, int numModels, bool standardBearers, bool hornblowers, int points);

        ~BlackKnights() override = default;

    protected:

        void onCharged() override;

        int rollChargeDistance() override;

    private:

        Weapon  m_barrowLance{Weapon::Type::Melee, "Barrow Lance", 2, 2, 4, 3, 0, 1},
                m_barrowLanceKnight{Weapon::Type::Melee, "Barrow Lance", 2, 3, 4, 3, 0, 1},
                m_hoovesAndTeeth{Weapon::Type::Melee, "Hooves and Teeth", 1, 2, 4, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Deathly Charge                   Yes
//


} //namespace Soulblight

