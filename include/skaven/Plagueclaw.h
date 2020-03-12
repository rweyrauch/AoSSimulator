/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUECLAW_H
#define PLAGUECLAW_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Plagueclaw : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 160;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    Plagueclaw();

    ~Plagueclaw() override = default;

    bool configure();

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_catapult,
        m_knives;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Barrage of Disease               Yes
// Hideous Death
//

} // namespace Skaven

#endif //PLAGUECLAW_H