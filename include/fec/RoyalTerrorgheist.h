/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROYALTERRORGHEIST_H
#define ROYALTERRORGHEIST_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class RoyalTerrorgheist : public FleshEaterCourts
{
public:
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    RoyalTerrorgheist();
    ~RoyalTerrorgheist() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

    int move() const override;
    void onSlain() override;

protected:

    void onStartHero(PlayerId player) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_deathShriek,
        m_skeletalClaws,
        m_fangedMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Death Shriek                     No
// Gaping Maw                       Yes
// Infested                         Yes
// Royal Menagerie                  Yes
//

} // namespace FleshEaterCourt

#endif //ROYALTERRORGHEIST_H