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

    class OgroidMyrmidon : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        explicit OgroidMyrmidon(DamnedLegion legion);

        OgroidMyrmidon() = delete;

        ~OgroidMyrmidon() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_spearMissile{Weapon::Type::Missile, "Gladiator Spear", 18, 1, 3, 3, -1, RAND_D3},
                m_spear{Weapon::Type::Melee, "Gladiator Spear", 2, 6, 3, 3, -1, 1},
                m_horns{Weapon::Type::Melee, "Great Horns", 1, 1, 3, 3, -2, 3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Fury                      Yes
// Berserk Rage                     TODO
// Pit Marshall                     Yes
//

} // SlavesToDarkness

