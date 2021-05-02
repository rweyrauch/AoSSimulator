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

    class FellwaterTroggoths : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FellwaterTroggoths(int points);

        ~FellwaterTroggoths() override = default;

        bool configure(int numModels);

    protected:

        void onStartHero(PlayerId player) override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_noxiousVomit,
                m_spikedClub;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Regeneration                     Yes
// Terrible Stench                  Yes
//

} // namespace GloomspiteGitz
