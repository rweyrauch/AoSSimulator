/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GLAIVEWRAITHSTALKERS_H
#define GLAIVEWRAITHSTALKERS_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class GlaivewraithStalkers : public Nighthaunt
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 16;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels);
    static void Init();

    GlaivewraithStalkers();

    ~GlaivewraithStalkers() override = default;

    bool configure(int numModels, bool drummer);

protected:

    bool m_drummer = true;

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

private:

    Weapon m_huntersGlaive;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// The Point of Death               Yes
//

} // namespace Nighthaunt

#endif // GLAIVEWRAITHSTALKERS_H
