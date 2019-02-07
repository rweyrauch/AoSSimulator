/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODTHIRSTERRAGE_H
#define BLOODTHIRSTERRAGE_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class BloodthirsterOfInsensateRage : public Unit
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    BloodthirsterOfInsensateRage();
    ~BloodthirsterOfInsensateRage() override = default;

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

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