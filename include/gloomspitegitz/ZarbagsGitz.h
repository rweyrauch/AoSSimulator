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

namespace GloomspiteGitz {

    class ZarbagsGitz : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        ZarbagsGitz();

        ~ZarbagsGitz() override = default;

        bool configure();

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
// Abilities                    Implemented
// -------------------------------------------
// Squigs Go Wild                   TODO
// Tough as Old Boots               Yes
// Netters                          Yes
// Loonsmasha Fanatic               TODO
//

} // namespace GloomspiteGitz

#endif //ZARBAGSGITZ_H