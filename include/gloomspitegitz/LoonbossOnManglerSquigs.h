/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOONBOSSMANGLERSQUIGS_H
#define LOONBOSSMANGLERSQUIGS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class LoonbossOnManglerSquigs : public Unit
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 12;

    LoonbossOnManglerSquigs();
    ~LoonbossOnManglerSquigs() override = default;

    bool configure();

protected:

    int toHitModifier(const Unit* unit) const override;

    static Weapon s_hugeFangFilledGob,
        s_moonCutta,
        s_ballsAndChains,
        s_grotsBashinStikk;
};

} // namespace GloomspiteGitz

#endif //LOONBOSSMANGLERSQUIGS_H