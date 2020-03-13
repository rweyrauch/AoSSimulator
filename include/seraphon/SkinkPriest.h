/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKINKPRIEST_H
#define SKINKPRIEST_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SkinkPriest : public SeraphonBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 70;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SkinkPriest();
    ~SkinkPriest() override = default;

    bool configure();

protected:

private:

    Weapon m_starbolt,
        m_staff;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Star-stone Staff                 TODO
// Herald of the Old Ones           TODO
//

} // namespace Seraphon

#endif //SKINKPRIEST_H