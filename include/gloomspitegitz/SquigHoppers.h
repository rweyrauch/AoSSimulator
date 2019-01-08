/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SQUIGHOPPERS_H
#define SQUIGHOPPERS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class SquiqHoppers : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;

    SquiqHoppers();
    ~SquiqHoppers() override = default;

    bool configure(int numModels);

protected:

    static Weapon s_fangFilledGob,
        s_slitta,
        s_slittaBoss;
};

} // namespace GloomspiteGitz

#endif //SQUIGHOPPERS_H