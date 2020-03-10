/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROGUEIDOL_H
#define ROGUEIDOL_H

#include <Unit.h>
#include <Weapon.h>

namespace Greenskinz
{

class RogueIdol : public Unit
{
public:

    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 400;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    RogueIdol();
    ~RogueIdol() override = default;

    bool configure();

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    void onRestore() override;

    void onSlain() override;
    Wounds applyWoundSave(const Wounds &wounds) override;
    Wounds onEndCombat(PlayerId player) override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_boulderFists,
        m_stompinFeet;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Avalanche!                       Yes
// Da Big' Un                       Yes
// Livin' Idol                      No
// Rubble and Ruin                  Yes
// Spirit of the Waaagh!            Yes
//

} // namespace Greenskinz

#endif // ROGUEIDOL_H