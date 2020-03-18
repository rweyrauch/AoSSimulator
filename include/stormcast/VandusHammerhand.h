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
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    VandusHammerhand();
    ~VandusHammerhand() override;

    bool configure();

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    int lordOfTheHammerhandsBraveryMod(const Unit* unit);

private:

    Weapon m_heldensen,
        m_clawsAndFangs;

    lsignal::slot m_lordSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Heldensen                        Yes
// Intolerable Damage               Yes
// Storm Breath                     TODO
// Lord of the Hammerhands          Yes
// Vengeful Determination           TODO
//

} // namespace StormcastEternals

#endif //VANDUSHAMMERHAND_H