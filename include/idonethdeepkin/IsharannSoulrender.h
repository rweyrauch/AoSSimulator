/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SOULRENDER_H
#define SOULRENDER_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class IsharannSoulrender : public IdonethDeepkinBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    IsharannSoulrender();
    ~IsharannSoulrender() override = default;

    bool configure();

protected:


private:

    Weapon m_talunhook,
        m_bill;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Lurelight                        TODO
// Hangman's Knot                   TODO
//

} // namespace IdonethDeepkin

#endif // SOULRENDER_H
