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
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    DankholdTroggboss();
    ~DankholdTroggboss() override;

    bool configure();

protected:

    void onStartCombat(PlayerId player) override;
    void onStartHero(PlayerId player) override;
    int reassuringPresence(const Unit* unit);

private:

    Weapon m_boulderClub;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Crushing Grip                    Yes
// Magical Resistance               TODO
// Reassuring Presence              Yes
// Regeneration                     Yes
// Squiggly-beast Followers         Yes
// Instinctive Leader               TODO
//

} // namespace GloomspiteGitz

#endif //DANKHOLDTROGGBOSS_H