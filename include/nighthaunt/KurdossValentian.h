/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KURDOSSVALENTIAN_H
#define KURDOSSVALENTIAN_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class KurdossValentian : public Nighthaunt
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    KurdossValentian();
    ~KurdossValentian() override = default;

    bool configure();

protected:

private:

    Weapon m_sceptre,
        m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  No
// If I Cannot Rule, ...            No
// Soul-crushing Smite              No
// Suffer No Rival                  No
//


} // namespace Nighthaunt

#endif // KURDOSSVALENTIAN_H
