/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed {

    class WardenKing : public Dispossessed {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        WardenKing();

        ~WardenKing() override = default;

        bool configure();

    protected:

        // Ancestor Shield
        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override { return Rerolls::Failed; }

    private:

        Weapon m_runeWeapon;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ancestor Shield                  Yes
// Oath Stone                       TODO
// Ancestral Grudge                 TODO
//

} // namespace Dispossessed
