/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_ALARIELLE_H
#define WARHAMMERSIM_ALARIELLE_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Alarielle : public Unit
{
public:

    static const int BASESIZE = 160;
    static const int WOUNDS = 16;

    Alarielle();
    ~Alarielle() override = default;

    bool configure();

protected:

    int toHitModifier(const Unit& unit) const override;

private:

    static Weapon s_spearOfKurnoth,
        s_talonOfDwindling,
        s_beetleGreatAntlers;
};

} // namespace Sylvaneth

#endif //WARHAMMERSIM_ALARIELLE_H