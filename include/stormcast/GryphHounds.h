/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRYPHHOUNDS_H
#define GRYPHHOUNDS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class GryphHounds : public StormcastEternal
{
public:

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels);
    static void Init();

    GryphHounds();

    ~GryphHounds() override = default;

    bool configure(int numModels);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_beakAndClaws,
        m_beakAndClawsAlpha;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Loyal Companion                  Yes
// Darting Attacks                  TODO
// Warning Cry                      TODO
//

} // namespace StormcastEternals

#endif //GRYPHHOUNDS_H