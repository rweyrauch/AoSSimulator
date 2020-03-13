/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FATESKIMMER_H
#define FATESKIMMER_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class Fateskimmer : public TzeentchBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Fateskimmer();
    ~Fateskimmer() override = default;

    bool configure();

protected:

private:

    Weapon m_magicalFlames,
        m_staff,
        m_dagger,
        m_bite;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Tome                      TODO
// Sky-shark                        TODO
// Wake of Fire                     TODO
// Tzeentch's Firestorm             TODO
//

} // namespace Tzeentch

#endif //FATESKIMMER_H
