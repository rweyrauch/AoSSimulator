/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class IcefallYhetees : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        IcefallYhetees() = delete;

        ~IcefallYhetees() override = default;

    protected:

        IcefallYhetees(Mawtribe tribe, int numModels, int points);

    protected:

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_clawsAndClubs{Weapon::Type::Melee, "Claws and Ice-encrusted Clubs", 1, 3, 4, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Frost                    Yes
// Bounding Leaps                   Yes
// Invigorated by the Blizzard      TODO
//

} // namespace OgorMawtribes

