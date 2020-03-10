/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DREADSAURIAN_H
#define DREADSAURIAN_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class DreadSaurian : public SeraphonBase
{
public:

    static const int BASESIZE = 280; // x210 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 420;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    DreadSaurian();
    ~DreadSaurian() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

    void onStartHero(PlayerId player) override;

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_gargantuanJaws,
        m_rakingClaws,
        m_armouredTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Devourer of Beasts               Yes
// Roar of Ruin                     No
// Arcane Glyphs                    Yes
// Primal Presence                  No
//

} // namespace Seraphon

#endif //DREADSAURIAN_H
