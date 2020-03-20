/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VANGUARDRAPTORSLONGSTRIKE_H
#define VANGUARDRAPTORSLONGSTRIKE_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class VanguardRaptorsLongstrike : public StormcastEternal
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    VanguardRaptorsLongstrike();
    ~VanguardRaptorsLongstrike() override = default;

    bool configure(int numModels);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartShooting(PlayerId player) override;

private:

    Weapon m_longstikeCrossbow,
                  m_heavyStock,
                  m_beakAndClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Headshot                         Yes
// Hunting Call                     TODO
// Longshot                         Yes
//

} // namespace StormcastEternals

#endif //VANGUARDRAPTORSLONGSTRIKE_H