/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLMASTER_H
#define SKULLMASTER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Skullmaster : public KhorneBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Skullmaster();
    ~Skullmaster() override = default;

    bool configure();

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onCharged() override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_bladeOfBlood,
        m_brazenHooves;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Slaughter and Ruin               Yes
// Decapitating Blow                Yes
// Slaughterous Charge              Yes
//

} // namespace Khorne

#endif //SKULLMASTER_H