/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTVENATOR_H
#define KNIGHTVENATOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class KnightVenator : public StormcastEternal
{
public:

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    KnightVenator();
    ~KnightVenator() override = default;

    bool configure();

protected:

    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_realmhuntersBow,
        m_beakAndTalonsMissile,
        m_bowStave,
        m_beakAndTalons;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Celestial Strike                 Yes
// Star-fated Arrow                 TODO
//

} // namespace StormcastEternals

#endif //KNIGHTVENATOR_H