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

    class ManglerSquigs : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ManglerSquigs();

        ~ManglerSquigs() override = default;

    protected:

        void configure();

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        void onWounded() override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_hugeFangFilledGob,
                m_ballsAndChains,
                m_grotsBashinStikk;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ker-splat!                       Yes
// Watch Out!                       Yes
//

} // namespace GloomspiteGitz
