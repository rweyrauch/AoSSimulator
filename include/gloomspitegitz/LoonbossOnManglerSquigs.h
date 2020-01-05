/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOONBOSSMANGLERSQUIGS_H
#define LOONBOSSMANGLERSQUIGS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class LoonbossOnManglerSquigs : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 80;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    LoonbossOnManglerSquigs();
    ~LoonbossOnManglerSquigs() override = default;

    int move() const override;

    bool configure();

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    void onWounded() override;
    void onSlain() override;

    void onStartHero(PlayerId player) override;

private:

    int getDamageTableIndex() const;

    bool m_eatenRedcapMushroom = false;
    Rerolls m_toHitRerolls = NoRerolls;
    Rerolls m_toWoundRerolls = NoRerolls;

    Weapon m_hugeFangFilledGob,
        m_moonCutta,
        m_ballsAndChains,
        m_grotsBashinStikk;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ker-splat!                       Yes
// Redcap Mushrooms                 Yes
// Watch Out!                       Yes
// Bit Da Moon!                     No
//

} // namespace GloomspiteGitz

#endif //LOONBOSSMANGLERSQUIGS_H