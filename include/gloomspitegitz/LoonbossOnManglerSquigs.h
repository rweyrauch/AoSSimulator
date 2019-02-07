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
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    LoonbossOnManglerSquigs();
    ~LoonbossOnManglerSquigs() override = default;

    void hero(PlayerId player) override;
    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    void onWounded() override;
    void onSlain() override;

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