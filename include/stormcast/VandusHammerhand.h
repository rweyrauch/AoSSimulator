/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VANDUSHAMMERHAND_H
#define VANDUSHAMMERHAND_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class VandusHammerhand : public StormcastEternal
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    VandusHammerhand();
    ~VandusHammerhand() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int extraAttacks(const Weapon *weapon) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_heldensen,
        m_clawsAndFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Heldensen                        Yes
// Intolerable Damage               Yes
// Storm Breath                     No
// Lord of the Hammerhands          No
// Vengeful Determination           No
//

} // namespace StormcastEternals

#endif //VANDUSHAMMERHAND_H