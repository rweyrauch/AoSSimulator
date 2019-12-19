/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ZARBAGSGITZ_H
#define ZARBAGSGITZ_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class ZarbagsGitz : public GloomspiteGitzBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int SQUIG_WOUNDS = 2;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ZarbagsGitz();
    ~ZarbagsGitz() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_bow,
        m_teeth,
        m_prodder,
        m_slitta;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Squigs Go Wild                   No
// Tough as Old Boots               Yes
// Netters                          Yes
// Loonsmasha Fanatic               No
//

} // namespace GloomspiteGitz

#endif //ZARBAGSGITZ_H