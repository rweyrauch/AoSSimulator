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

    class Flagellants : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Flagellants(City city, int numModels, int points);

        ~Flagellants() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        void onFlee(int numFled) override;

    private:

        Weapon m_flailsAndClubs,
                m_flailsAndClubsProphet;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Glorious Martyrs                 Yes
// Fanatical Fury                   Yes
// Reckless Abandon                 Yes
//

} // namespace CitiesOfSigmar
