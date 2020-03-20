/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GUARDIANOFSOULS_H
#define GUARDIANOFSOULS_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class GuardianOfSouls : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GuardianOfSouls();

        ~GuardianOfSouls() override = default;

        bool configure();

    protected:

    private:

        Weapon m_blade,
                m_maul;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Nightmare Lantern                TODO
// Spectral Lure                    TODO
//


} // namespace Nighthaunt

#endif // GUARDIANOFSOULS_H
