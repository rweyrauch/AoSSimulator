/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONSKULLSBOYZ_H
#define IRONSKULLSBOYZ_H

#include <ironjawz/Ironjawz.h>
#include <Weapon.h>

namespace Ironjawz
{

class IronskullsBoyz : public Ironjawz
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    IronskullsBoyz();
    ~IronskullsBoyz() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

protected:

    Weapon m_eadButt,
        m_pairedChoppas,
        m_bigChoppa;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Dead 'Ard                        Yes
// Paired Choppas                   Yes
//

} // namespace Ironjawz

#endif // IRONSKULLSBOYZ_H