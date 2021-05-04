/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class KrulghastCruciator : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KrulghastCruciator(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral);

        KrulghastCruciator() = delete;

        ~KrulghastCruciator() override = default;

    protected:

    private:

        Weapon  m_torture{Weapon::Type::Missile, "Phantasmal Torture", 12, 4, 3, 3, -2, 1},
                m_talonsAndKnives{Weapon::Type::Melee, "Talons and Flensing Knives", 1, 4, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Empowering Excruciation          TODO
//

} // namespace Nighthaunt

