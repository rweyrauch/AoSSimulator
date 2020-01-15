/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THEGLOTTKIN_H
#define THEGLOTTKIN_H

#include <Unit.h>
#include <Weapon.h>

namespace Nurgle
{

class TheGlottkin : public Unit
{
public:

    static const int BASESIZE = 130;
    static const int WOUNDS = 18;
    static const int POINTS_PER_UNIT = 420;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    TheGlottkin();
    ~TheGlottkin() override = default;

    int move() const override;

    bool configure();

protected:

    void onStartHero(PlayerId player) override;
    void onWounded() override;
    void onCharged() override;
    int getDamageTableIndex() const;

private:

    Weapon m_pestilentTorrent,
        m_flailingTentacle,
        m_lampreyMaw,
        m_poisonScythe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Mountain of Loathsome Flesh      Yes
// Blessing of Nurgle               Yes
// Horrific Opponent                No
// Fleshy Abundance                 No
// Lords of Nurgle                  No
//

} // Nurgle

#endif //THEGLOTTKIN_H