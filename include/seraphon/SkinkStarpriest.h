/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKINKSTARPRIEST_H
#define SKINKSTARPRIEST_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SkinkStarpriest : public SeraphonBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SkinkStarpriest();
    ~SkinkStarpriest() override = default;

    bool configure();

protected:

private:

    Weapon m_venombolt,
        m_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Astral Herald                    No
// Serpent Staff                    No
// Blazing Starlight                No
//

} // namespace Seraphon

#endif //SKINKSTARPRIEST_H