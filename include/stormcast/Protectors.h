/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PROTECTORS_H
#define PROTECTORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Protectors : public StormcastEternal
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Protectors();
    ~Protectors() override = default;

    bool configure(int numModels, int numStarsoulMaces);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_glaive,
        m_glaivePrime,
        m_starsoulMace;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Deathstrike                      Yes
// Storm-shield                     Partial/TODO
// Starsoul Mace                    Yes
//

} // namespace StormcastEternals

#endif //PROTECTORS_H