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

    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Protectors();
    ~Protectors() override = default;

    bool configure(int numModels, int numStarsoulMaces);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deathstrike                      No
// Storm-shield                     No
// Starsoul Mace                    Yes
//

} // namespace StormcastEternals

#endif //PROTECTORS_H