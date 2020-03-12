/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRAGONOGORS_H
#define DRAGONOGORS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class DragonOgors : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    DragonOgors();
    ~DragonOgors() override = default;

    bool configure(int numModels, int numPairedWeapons, int numGlaives, int numCrushers);

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_pairedAncientWeapons,
        m_draconicWarglaive,
        m_draconicCrusher,
        m_rakingForeclaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Storm Rage                       Yes
//

} // namespace BeastsOfChaos

#endif //DRAGONOGORS_H