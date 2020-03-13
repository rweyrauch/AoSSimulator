/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FATEMASTER_H
#define FATEMASTER_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class Fatemaster : public TzeentchBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Fatemaster();
    ~Fatemaster() override = default;

    bool configure();

protected:

private:

    Weapon m_glaive,
        m_teethAndHorns;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Hovering Disc of Tzeentch        TODO
// Soulbound Shield                 TODO
// Lord of Fate                     TODO
//

} // namespace Tzeentch

#endif //FATEMASTER_H
