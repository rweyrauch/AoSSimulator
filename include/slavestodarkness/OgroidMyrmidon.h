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

        OgroidMyrmidon();

        ~OgroidMyrmidon() override = default;

    protected:

        void configure();

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_spearMissile,
                m_spear,
                m_horns;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Fury                      Yes
// Berserk Rage                     TODO
// Pit Marshall                     TODO
//

} // SlavesToDarkness

