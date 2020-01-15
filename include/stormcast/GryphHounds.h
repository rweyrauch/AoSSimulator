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

    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 6;
    static const int MAX_UNIT_SIZE = 18;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 420;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Loyal Companion                  Yes
// Darting Attacks                  No
// Warning Cry                      No
//

} // namespace StormcastEternals

#endif //GRYPHHOUNDS_H