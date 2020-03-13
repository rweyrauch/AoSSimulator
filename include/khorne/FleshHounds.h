/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLESHHOUNDS_H
#define FLESHHOUNDS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class FleshHounds : public KhorneBase
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    FleshHounds();
    ~FleshHounds() override = default;

    bool configure(int numModels);

protected:

    // Unflagging Hunter
    Rerolls chargeRerolls() const override { return RerollFailed; }

private:


    Weapon m_burningRoar,
        m_blooddarkClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Collars of Khorne                TODO
// Unflagging Hunters               Yes
//

} // namespace Khorne

#endif //FLESHHOUNDS_H