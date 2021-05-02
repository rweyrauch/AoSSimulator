/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class BloodSisters : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BloodSisters(int points);

        ~BloodSisters() override = default;

        bool configure(int numModels);

    protected:

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_heartshardGlaive,
                m_heartshardGlaiveGorgai;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Turned to Crystal                Yes
//

} // namespace DaughtersOfKhaine
