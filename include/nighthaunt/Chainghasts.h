/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAINGHASTS_H
#define CHAINGHASTS_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class Chainghasts : public Nighthaunt
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Chainghasts();
    ~Chainghasts() override = default;

    bool configure(int numModels);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_ghastflailsMissile,
        m_ghastflails;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Another Link in the Chain        TODO
// Sweeping Blows                   Yes


} // namespace Nighthaunt

#endif // CHAINGHASTS_H
