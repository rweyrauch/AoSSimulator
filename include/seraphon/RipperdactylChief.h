/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class RipperdactylChief : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        RipperdactylChief(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral);

        RipperdactylChief() = delete;

        ~RipperdactylChief() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_skyblade{Weapon::Type::Melee, "Skyblade", 1, 4, 3, 4, -1, 1},
                m_jaws{Weapon::Type::Melee, "Tearing Jaws", 1, 4, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Voracious Appetite               Yes
// Ripperdactyl Assault             TODO
//

} // namespace Seraphon
