/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class Slaughterbrute : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Slaughterbrute(DamnedLegion legion, MarkOfChaos mark);

        Slaughterbrute() = delete;

        ~Slaughterbrute() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon  m_claws{Weapon::Type::Melee, "Razor-tipped Claws", 2, 6, 4, 3, -1, RAND_D3},
                m_jaws{Weapon::Type::Melee, "Mighty Jaws", 1, 2, 4, 1, 0, 3},
                m_talons{Weapon::Type::Melee, "Slashing Talons", 1, 2, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Sigils of Enslavement            TODO
// Beast Unbound                    TODO
//

} // SlavesToDarkness

