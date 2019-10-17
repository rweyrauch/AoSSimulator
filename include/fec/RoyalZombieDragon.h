/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROYALZOMBIEDRAGON_H
#define ROYALZOMBIEDRAGON_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class RoyalZombieDragon : public FleshEaterCourts
{
public:
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    RoyalZombieDragon();
    ~RoyalZombieDragon() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_pestilentialBreath,
        m_snappingMaw,
        m_swordlikeClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Pestilential Breath              No
//

} // namespace FleshEaterCourt

#endif //ROYALZOMBIEDRAGON_H