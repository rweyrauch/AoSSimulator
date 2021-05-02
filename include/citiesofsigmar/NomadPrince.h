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

    class NomadPrince : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        NomadPrince();

        ~NomadPrince() override = default;

        bool configure();

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_spear;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Harrying Bird of Prey            Yes
// Lord of the Deepwood Host        Yes
//

} // namespace CitiesOfSigmar
