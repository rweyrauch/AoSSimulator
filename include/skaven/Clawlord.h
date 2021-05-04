/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Clawlord : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Clawlord(CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Clawlord() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_blade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cornered Fury                    Yes
// Gnash-gnaw on the Bones!         TODO
//

} // namespace Skaven
