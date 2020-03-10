/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GHOULKINGZOMBIEDRAGON_H
#define GHOULKINGZOMBIEDRAGON_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class AbhorrantGhoulKingOnZombieDragon : public FleshEaterCourts
{
public:
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 440;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    AbhorrantGhoulKingOnZombieDragon();
    ~AbhorrantGhoulKingOnZombieDragon() override = default;

    bool configure();

protected:

    void onStartHero(PlayerId player) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onWounded() override;
    void onRestore() override;

    int getDamageTableIndex() const;

private:

    Weapon m_pestilentialBreath,
        m_goryTalonsAndFangs,
        m_snappingMaw,
        m_swordlikeClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Pestilential Breath              No
// Royal Blood                      Yes
// Malefic Hunger                   No
// Summon Courtier                  No
//

} // namespace FleshEaterCourt

#endif //GHOULKINGZOMBIEDRAGON_H