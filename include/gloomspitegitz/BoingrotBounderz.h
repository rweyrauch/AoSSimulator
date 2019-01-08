/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BOINGROTBOUNDERZ_H
#define BOINGROTBOUNDERZ_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class BoingrotBounderz : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;

    BoingrotBounderz();
    ~BoingrotBounderz() override = default;

    bool configure(int numModels);

protected:

    static Weapon s_fangFilledGob,
        s_pokinLance,
        s_pokinLanceBoss;
};

} // namespace GloomspiteGitz

#endif //BOINGROTBOUNDERZ_H