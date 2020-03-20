/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GNOBLARS_H
#define GNOBLARS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Gnoblars : public MawtribesBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Gnoblars();
    ~Gnoblars() override = default;

    bool configure(int numModels);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_sharpStuff,
        m_motleyWeapons,
        m_motleyWeaponsBiter;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Screeching Horde                 Yes
// Trappers                         TODO
//

} // namespace OgorMawtribes

#endif //GNOBLARS_H
