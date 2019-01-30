/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROCKGUTTROGGOTHS_H
#define ROCKGUTTROGGOTHS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class RockgutTroggoths : public Unit
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 640;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    RockgutTroggoths();
    ~RockgutTroggoths() override = default;

    bool configure(int numModels);
    void hero(PlayerId player) override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onStartShooting(PlayerId player) override;
    Wounds applyWoundSave(const Wounds& wounds) override;

private:

    static Weapon s_massiveStoneMaul;

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