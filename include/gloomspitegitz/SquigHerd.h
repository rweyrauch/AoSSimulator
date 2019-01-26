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
    static const int MIN_UNIT_SIZE = 6;
    static const int MAX_UNIT_SIZE = 24;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SquiqHerd();
    ~SquiqHerd() override = default;

    bool configure(int numModels);

protected:

    bool hasHerder() const;

    Rerolls runRerolls() const override;
    Rerolls chargeRerolls() const override;
    void onFlee(int numFled) override;

private:

    static Weapon s_fangFilledGob,
        s_squigProdder;

    static bool s_registered;
};

} // namespace GloomspiteGitz

#endif //SQUIGHERD_H