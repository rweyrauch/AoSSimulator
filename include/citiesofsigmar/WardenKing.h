/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class WardenKing : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WardenKing(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~WardenKing() override = default;

    private:

        Weapon m_runeWeapon;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oath Stone                       TODO
// Ancestral Grudge                 TODO
//

} // namespace CitiesOfSigmar
