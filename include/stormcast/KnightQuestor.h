/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTQUESTOR_H
#define KNIGHTQUESTOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class KnightQuestor : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels)  { return POINTS_PER_UNIT; }

    KnightQuestor();
    ~KnightQuestor() override = default;

    bool configure();

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_warblade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thundercharged Strike            Yes
// Heroic Challenge                 Yes
// Sigmarite Shield                 Yes
//

} // namespace StormcastEternals

#endif //KNIGHTQUESTOR_H