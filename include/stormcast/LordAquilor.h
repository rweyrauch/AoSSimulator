/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDAQUILOR_H
#define LORDAQUILOR_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordAquilor : public StormcastEternal
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    LordAquilor();
    ~LordAquilor() override = default;

    bool configure(bool astralCompass);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    bool m_astralCompass = false;

    Weapon m_boltstormPistol,
        m_starboundBlade,
        m_shockHandaxe,
        m_beakAndClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Aethereal Strike                 Yes
// Ride the Winds Aetheric          TODO
// Astral Compass                   TODO
// Lord of the Azyrite Hurricane    TODO

} // namespace StormcastEternals

#endif //LORDAQUILOR_H