/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORGHOTTS_H
#define ORGHOTTS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class OrghottsDaemonspew : public NurgleBase
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;


    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    OrghottsDaemonspew();
    ~OrghottsDaemonspew() override = default;

    bool configure();

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_tongue,
        m_rotaxes,
        m_claws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Acid Ichor                       TODO
// Fury of the Halfblood            Yes
// The Rotaxes                      TODO
// Fester and Rot                   TODO
//

} // Nurgle

#endif //ORGHOTTS_H