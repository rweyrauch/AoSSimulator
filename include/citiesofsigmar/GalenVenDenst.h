/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class GalenVenDenst : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GalenVenDenst(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~GalenVenDenst() override = default;

    protected:


    private:

        Weapon  m_pistol{Weapon::Type::Missile, "Pistol", 9, 3, 3, 3, -1, 1},
                m_broadsword{Weapon::Type::Melee, "Broadsword", 1, 6, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Grim Resolve                     TODO
// Agile Opponent                   TODO
// Weapons of Banishment            TODO
//

} // namespace CitiesOfSigmar
