/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DARKOATHCHIEFTAIN_H
#define DARKOATHCHIEFTAIN_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class DarkoathChieftain : public SlavesToDarknessBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    DarkoathChieftain();
    ~DarkoathChieftain() override = default;

    bool configure();

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_axe,
        m_broadsword;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Berserk Charge                   Yes
// Deathblow                        TODO
// Last Gasp of Glory               TODO
//

} // SlavesToDarkness

#endif //DARKOATHCHIEFTAIN_H