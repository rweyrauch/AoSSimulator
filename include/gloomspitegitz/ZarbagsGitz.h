/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

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

    protected:

        void configure();

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onModelFled(const Model* model) override;

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
// Squigs Go Wild                   Yes
// Tough as Old Boots               Yes
// Netters                          Yes
// Loonsmasha Fanatic               TODO
//

} // namespace GloomspiteGitz
