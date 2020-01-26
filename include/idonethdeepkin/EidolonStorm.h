/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EIDOLONSTORM_H
#define EIDOLONSTORM_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class EidolonOfMathlannAspectOfTheStorm : public IdonethDeepkinBase
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 360;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    EidolonOfMathlannAspectOfTheStorm();
    ~EidolonOfMathlannAspectOfTheStorm() override = default;

    bool configure();

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onCharged() override;

private:

    Weapon m_spear,
        m_crulhook,
        m_stormshoal;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Crashing Upon the Foe            Yes
// Drench with Hate                 No
// Pulled Into the Depths           No
//

} // namespace IdonethDeepkin

#endif // EIDOLONSTORM_H
