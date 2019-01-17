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
    static const int BASESIZE = 80;
    static const int WOUNDS = 12;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    LoonbossOnManglerSquigs();
    ~LoonbossOnManglerSquigs() override
    {
        delete m_pHugeFangFilledGob;
        delete m_pBallsAndChains;
    }

    int move() const override;

    bool configure();

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    void onWounded() override;

    int getDamageTableIndex() const;

    Weapon* m_pHugeFangFilledGob = nullptr;
    Weapon* m_pBallsAndChains = nullptr;

    static Weapon s_hugeFangFilledGob,
        s_moonCutta,
        s_ballsAndChains,
        s_grotsBashinStikk;

    static bool s_registered;
};

} // namespace GloomspiteGitz

#endif //LOONBOSSMANGLERSQUIGS_H