/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTOFSHROUDS_H
#define KNIGHTOFSHROUDS_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class KnightOfShrouds : public Nighthaunt
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    KnightOfShrouds();
    ~KnightOfShrouds() override = default;

    bool configure();

protected:

private:

    Weapon m_sword;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Stolen Hours                     No
// Spectral Overseer                No
//


} // namespace Nighthaunt

#endif // KNIGHTOFSHROUDS_H
