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

    class SaurusEternityWarden : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusEternityWarden(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral);

        SaurusEternityWarden() = delete;

        ~SaurusEternityWarden() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_mace{Weapon::Type::Melee, "Star-stone Mace", 1, 3, 3, 3, -1, 1},
                m_jaws{Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cold Ferocity                    Yes
// Selfless Protectors              Yes (see Lord Kroak and Slann Starmasters)
// Prime Guardian                   TODO
//

} // namespace Seraphon
