/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TOMBBANSHEE_H
#define TOMBBANSHEE_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class TombBanshee : public Nighthaunt
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    TombBanshee();
    ~TombBanshee() override = default;

    bool configure();

protected:

private:

    Weapon m_dagger;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  No
// Ghostly Howl                     No
//


} // namespace Nighthaunt

#endif // TOMBBANSHEE_H
