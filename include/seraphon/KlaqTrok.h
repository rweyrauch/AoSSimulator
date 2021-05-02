/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class KlaqTrok : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit KlaqTrok(bool isGeneral);

        ~KlaqTrok() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_warblade{Weapon::Type::Melee, "Celestite Warblade", 1, 5, 3, 3, 0, 1},
                m_jaws{Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bolstering Presence              Yes (see StarbloodStalkers)
// Cold Ferocity                    Yes
//

} // namespace Seraphon
