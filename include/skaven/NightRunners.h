/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NIGHTRUNNERS_H
#define NIGHTRUNNERS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class NightRunners : public Skaventide
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    NightRunners();
    ~NightRunners() override = default;

    bool configure(int numModels);

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    Weapon m_throwingWeapons,
        m_stabbingBlade,
        m_stabbingBladeLeader;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Running Death                    Yes
// Slinking Advance                 No
// Throwing Weapons                 Yes
//

} // namespace Skaven

#endif //NIGHTRUNNERS_H