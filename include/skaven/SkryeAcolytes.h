/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class SkryeAcolytes : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkryeAcolytes();

        ~SkryeAcolytes() override = default;

        bool configure(int numModels);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_globe,
                m_knife;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Quick-quick Volley!              Yes
// Gas Clouds                       Yes
//

} // namespace Skaven
