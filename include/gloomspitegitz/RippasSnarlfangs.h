/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SNARLFANGS_H
#define SNARLFANGS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class RippasSnarlfangs : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    RippasSnarlfangs();
    ~RippasSnarlfangs() override = default;

    bool configure();

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_grotBow,
        m_bossLoppa,
        m_stikka,
        m_bowStave,
        m_jaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Smell Weakness                   Yes
// Ferocious Pounce                 Yes
//

} // namespace GloomspiteGitz

#endif //SNARLFANGS_H