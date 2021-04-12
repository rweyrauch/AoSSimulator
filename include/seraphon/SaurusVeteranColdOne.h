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

    class SaurusScarVeteranOnColdOne : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusScarVeteranOnColdOne(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral);

        SaurusScarVeteranOnColdOne() = delete;

        ~SaurusScarVeteranOnColdOne() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_warpick{Weapon::Type::Melee, "Celestite Warpick", 1, 3, 3, 3, -1, 1},
                m_jaws{Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1},
                m_coldOneJaws{Weapon::Type::Melee, "Snapping Jaws", 1, 2, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cold Ferocity                    Yes
// Saurian Savagery                 TODO

} // namespace Seraphon
