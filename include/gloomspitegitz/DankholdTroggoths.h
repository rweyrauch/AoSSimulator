/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DANKHOLDTROGGOTHS_H
#define DANKHOLDTROGGOTHS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class DankholdTroggoths : public Unit
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 10;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 220;
    static const int POINTS_MAX_UNIT_SIZE = 660;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DankholdTroggoths();
    ~DankholdTroggoths() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onStartCombat(PlayerId player) override;
    void onStartHero(PlayerId player) override;

private:

    Weapon m_boulderClub;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Crushing Grip                    Yes
// Magical Resistance               No
// Reassuring Presence              No
// Regeneration                     Yes
// Squiggly-beast Followers         Yes
//

} // namespace GloomspiteGitz

#endif //DANKHOLDTROGGOTHS_H