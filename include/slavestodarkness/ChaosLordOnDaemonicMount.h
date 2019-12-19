/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDONMOUNT_H
#define LORDONMOUNT_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosLordOnDaemonicMount : public Unit
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 170;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ChaosLordOnDaemonicMount();
    ~ChaosLordOnDaemonicMount() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_hammer,
        m_hooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Chaos Runeshield                 Yes
// Cursed Warhammer                 No
// Fuelled by Carnage               No
// The Knights of Chaos             No
// Mark of Chaos                    No
//

} // SlavesToDarkness

#endif //LORDONMOUNT_H