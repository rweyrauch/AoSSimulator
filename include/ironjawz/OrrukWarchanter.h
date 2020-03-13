/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKWARCHANTER_H
#define ORRUKWARCHANTER_H

#include "Ironjawz.h"
#include <Weapon.h>

namespace Ironjawz
{

class OrrukWarchanter : public Ironjawz
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    OrrukWarchanter();
    ~OrrukWarchanter() override = default;

    bool configure();

protected:

private:

    Weapon m_stikks;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Rhythm of Destruction            TODO
// Violent Fury                     TODO

} // namespace Ironjawz

#endif // ORRUKWARCHANTER_H