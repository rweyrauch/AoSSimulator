/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEPRIEST_H
#define PLAGUEPRIEST_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class PlaguePriest : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    PlaguePriest();

    ~PlaguePriest() override = default;

    bool configure();

protected:

private:

    Weapon m_staff,
        m_censer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Plaque Prayers
//   Disease-disease!
//   Pestilence-pestilence!
// Frenzied Assault
// Poisonous Fumes
//

} // namespace Skaven

#endif //PLAGUEPRIEST_H