/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class DarkoathWarqueen : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DarkoathWarqueen(DamnedLegion legion, CommandTrait trait, Artefact artefact, bool isGeneral);

        DarkoathWarqueen() = delete;

        ~DarkoathWarqueen() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_axe{Weapon::Type::Melee, "Rune-etched Axe", 1, 6, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Infernal Runeshield              TODO
// Savage Duellist                  Yes
// The Will of the Gods             TODO
//

} // SlavesToDarkness
