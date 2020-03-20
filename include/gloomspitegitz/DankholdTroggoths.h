/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DANKHOLDTROGGOTHS_H
#define DANKHOLDTROGGOTHS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class DankholdTroggoths : public GloomspiteGitzBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    DankholdTroggoths();
    ~DankholdTroggoths() override;

    bool configure(int numModels);

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
//

} // namespace GloomspiteGitz

#endif //DANKHOLDTROGGOTHS_H