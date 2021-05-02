/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SaurusSunblood : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SaurusSunblood(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral);

        SaurusSunblood() = delete;

        ~SaurusSunblood() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_celestiteWarmace{Weapon::Type::Melee, "Celestite Warmace", 1, 6, 3, 3, -1, 1},
                m_jaws{Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Primal Rage                      Yes
// Scent of Weakness                TODO
//

} // namespace Seraphon
