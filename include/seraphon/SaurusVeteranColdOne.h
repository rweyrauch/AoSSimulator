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

        SaurusScarVeteranOnColdOne();

        ~SaurusScarVeteranOnColdOne() override = default;

        bool configure();

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_warpick,
                m_jaws,
                m_coldOneJaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cold Ferocity                    Yes
// Saurian Savagery                 TODO

} // namespace Seraphon
