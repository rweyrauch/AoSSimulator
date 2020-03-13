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
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    AbhorrantGhoulKing();
    ~AbhorrantGhoulKing() override = default;

    bool configure();

protected:

    void onStartHero(PlayerId player) override;

private:

    Weapon m_goryTalonsAndFangs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Royal Blood                      Yes
// Black Hunger                     TODO
// Summon Men-at-arms               TODO
//

} // namespace FleshEaterCourt

#endif //ABHORRANTGHOULKING_H