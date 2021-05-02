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

    class WildwoodRangers : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WildwoodRangers(int points);

        ~WildwoodRangers() override = default;

        bool configure(int numModels, bool standardBearer, bool hornblower);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

    private:

        Weapon m_rangersDraich,
                m_wardensDraich;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Guardians of the Kindreds        Yes
// Standard Bearer                  Yes
// Hornblower                       Yes
//

} // namespace Wanderers
