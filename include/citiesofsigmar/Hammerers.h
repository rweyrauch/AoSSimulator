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

    class Hammerers : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Hammerers(City city, int numModels, bool standardBearer, bool musician, int points);

        ~Hammerers() override = default;

    protected:

        bool battleshockRequired() const override;

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

    private:

        Weapon m_greatHammer,
                m_greatHammerKeeper;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Kingsguard                       Yes
// Musician                         Yes
// Standard Bearer                  Yes
// Musician                         Yes
//

} // namespace CitiesOfSigmar
