/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROCKGUTTROGGOTHS_H
#define ROCKGUTTROGGOTHS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class RockgutTroggoths : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 140*4;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    RockgutTroggoths();
    ~RockgutTroggoths() override = default;

    bool configure(int numModels);

protected:

    void onStartHero(PlayerId player) override;
    void onStartShooting(PlayerId player) override;
    Wounds applyWoundSave(const Wounds& wounds) override;

private:

    Weapon m_massiveStoneMaul;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Regeneration                     Yes
// Stony Skin                       Yes
// Throwin' Boulders                Yes
//

} // namespace GloomspiteGitz

#endif //ROCKGUTTROGGOTHS_H