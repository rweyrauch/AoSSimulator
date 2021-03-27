/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class Branchwraith : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Branchwraith();

        ~Branchwraith() override = default;

    protected:

        void configure(Lore lore);

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_piercingTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blessing of the Forest           Yes
// Roused to Wrath                  TODO
//

} // namespace Sylvaneth
