/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CLAWLORD_H
#define CLAWLORD_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Clawlord : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    Clawlord();

    ~Clawlord() override = default;

    bool configure();

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_blade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cornered Fury                    Yes
// Gnash-gnaw on the Bones!         No
//

} // namespace Skaven

#endif //CLAWLORD_H