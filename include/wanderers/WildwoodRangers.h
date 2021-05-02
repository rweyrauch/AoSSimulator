/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <wanderers/Wanderer.h>
#include <Weapon.h>

namespace Wanderers {

    class WildwoodRangers : public Wanderer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WildwoodRangers(int numModels, bool standardBearer, bool hornblower, int points);

        ~WildwoodRangers() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls runRerolls() const override;

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
