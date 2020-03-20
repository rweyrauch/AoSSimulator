/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GLAIVEWRAITHSTALKERS_H
#define GLAIVEWRAITHSTALKERS_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class GlaivewraithStalkers : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GlaivewraithStalkers();

        ~GlaivewraithStalkers() override = default;

        bool configure(int numModels, bool drummer);

    protected:

        bool m_drummer = true;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_huntersGlaive;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// The Point of Death               Yes
//

} // namespace Nighthaunt

#endif // GLAIVEWRAITHSTALKERS_H
