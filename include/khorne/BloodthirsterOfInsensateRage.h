/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODTHIRSTERRAGE_H
#define BLOODTHIRSTERRAGE_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class BloodthirsterOfInsensateRage : public KhorneBase
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 270;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    BloodthirsterOfInsensateRage();
    ~BloodthirsterOfInsensateRage() override = default;

    bool configure();

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onRestore() override;

private:

    Weapon m_greatAxeOfKhorne;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Rage Unbound                     Yes
// Outrageous Carnage               Partial
// Bloodthirsty Charge              No
//

} // namespace Khorne

#endif //BLOODTHIRSTERRAGE_H