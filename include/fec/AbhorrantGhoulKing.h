/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ABHORRANTGHOULKING_H
#define ABHORRANTGHOULKING_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class AbhorrantGhoulKing : public FleshEaterCourts
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    AbhorrantGhoulKing();
    ~AbhorrantGhoulKing() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartHero(PlayerId player) override;

private:

    Weapon m_goryTalonsAndFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Royal Blood                      Yes
// Black Hunger                     No
// Summon Men-at-arms               No
//

} // namespace FleshEaterCourt

#endif //ABHORRANTGHOULKING_H