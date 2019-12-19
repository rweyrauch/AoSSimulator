/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DANKHOLDTROGGBOSS_H
#define DANKHOLDTROGGBOSS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class DankholdTroggboss : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 270;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DankholdTroggboss();
    ~DankholdTroggboss() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

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
// Instinctive Leader               No
//

} // namespace GloomspiteGitz

#endif //DANKHOLDTROGGBOSS_H