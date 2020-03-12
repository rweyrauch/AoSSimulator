/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COCKATRICE_H
#define COCKATRICE_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Cockatrice : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }

    Cockatrice();
    ~Cockatrice() override = default;

    bool configure();

protected:

    void onStartShooting(PlayerId player) override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_petrifyingGaze,
        m_viciousBeak,
        m_swordlikeTalons;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Petrifying Gaze                  Yes
// Maddened Ferocity                Yes
//

} // namespace BeastsOfChaos

#endif //COCKATRICE_H