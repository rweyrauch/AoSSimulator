/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NAMARTIREAVERS_H
#define NAMARTIREAVERS_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class NamartiReavers : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 260;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    NamartiReavers();
    ~NamartiReavers() override = default;

    bool configure(int numModels, int numIconBearers);

protected:

    void onStartShooting(PlayerId player) override;

    // Swift Tide
    Rerolls runRerolls() const override { return RerollFailed; }

protected:

    int m_numIconBearers = 0;

private:

    Weapon m_keeningBlade,
        m_whisperbowAimedFire,
        m_whisperbowStormFire;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Swift Tide                       Yes
// Fluid Firing Style               Yes
//

} // namespace IdonethDeepkin

#endif // NAMARTIREAVERSS_H
