/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class CurselingEyeOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CurselingEyeOfTzeentch(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        CurselingEyeOfTzeentch() = delete;

        ~CurselingEyeOfTzeentch() override = default;

    protected:

        Rerolls unbindingRerolls() const override {
            // Disrupter of the Arcane
            return Rerolls::Failed;
        }

    private:

        Weapon  m_sword{Weapon::Type::Melee, "Blazing Sword", 1, 3, 3, 4, -1, 1},
                m_flail{Weapon::Type::Melee, "Threshing Flail", 1, 3, 4, 3, 0, 1},
                m_staff{Weapon::Type::Melee, "Staff of Tzeentch", 2, 1, 5, 4, 0, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Disrupter of the Arcane          Yes
// Vessel of Chaos                  TODO
// Glean Magic                      TODO
//

} // namespace Tzeentch

