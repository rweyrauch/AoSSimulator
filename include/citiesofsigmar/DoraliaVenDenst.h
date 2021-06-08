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

    class DoraliaVenDenst : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DoraliaVenDenst(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~DoraliaVenDenst() override = default;

    protected:

    private:

        Weapon  m_crossbow{Weapon::Type::Missile, "Crossbow", 24, 1, 3, 3, -2, 2},
                m_sword{Weapon::Type::Melee, "Witch Hunter's Sword", 1, 3, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Grim Resolve                     TODO
// Sureshot                         TODO
// Weapons of Banishment            TODO
//

} // namespace CitiesOfSigmar
