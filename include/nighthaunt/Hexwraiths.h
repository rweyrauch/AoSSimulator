/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HEXWRAITHS_H
#define HEXWRAITHS_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class Hexwraiths : public Nighthaunt
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Hexwraiths();
    ~Hexwraiths() override = default;

    bool configure(int numModels);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_spectralScythe,
        m_hoovesAndTeeth,
        m_spectralScytheHellwraith;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// Spectral Hunters                 TODO


} // namespace Nighthaunt

#endif // HEXWRAITHS_H
