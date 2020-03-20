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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    RoyalZombieDragon();
    ~RoyalZombieDragon() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

private:

    Weapon m_pestilentialBreath,
        m_snappingMaw,
        m_swordlikeClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Pestilential Breath              TODO
//

} // namespace FleshEaterCourt

#endif //ROYALZOMBIEDRAGON_H