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

    class Loonboss : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Loonboss(Allegiance allegiance, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Loonboss() override = default;

    protected:

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_moonslicer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Dead Tricksy                     Yes
// I'm Da Boss, Now Stab 'Em Good   Yes
//

} // namespace GloomspiteGitz
