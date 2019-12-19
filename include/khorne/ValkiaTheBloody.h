/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VALKIATHEBLOODY_H
#define VALKIATHEBLOODY_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class ValkiaTheBloody : public KhorneBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ValkiaTheBloody();
    ~ValkiaTheBloody() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int targetWoundModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_slaupnir;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// The Gaze of Khorne               No
// The Spear Slaupnir               Yes
// Daemonshield                     Yes
// On Bloodstained Wings            No
//

} // namespace Khorne

#endif //VALKIATHEBLOODY_H