/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SQUIGHERD_H
#define SQUIGHERD_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class SquiqHerd : public Unit
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 2;

    SquiqHerd();
    ~SquiqHerd() override = default;

    bool configure(int numModels);

protected:

    static Weapon s_fangFilledGob,
        s_squigProdder;
};

} // namespace GloomspiteGitz

#endif //SQUIGHERD_H