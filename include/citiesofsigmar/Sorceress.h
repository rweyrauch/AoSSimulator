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

    class Sorceress : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Sorceress(City city, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Sorceress() override = default;

    private:

        Weapon m_witchstaff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood Sacrifice                  TODO
// Word of Pain                     Yes
// Command Underlings               Yes
//

} // namespace CitiesOfSigmar
