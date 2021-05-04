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

    class BlackGuard : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BlackGuard(City city, int numModels, bool standardBearer, bool drummer, int points);

        ~BlackGuard() override = default;

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_halberd, m_halberdCaptain;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Drummer                          Yes
// Elite Bodyguard                  Yes
//

} // namespace CitiesOfSigmar
