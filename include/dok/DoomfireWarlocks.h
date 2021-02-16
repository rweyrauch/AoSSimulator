/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class DoomfireWarlocks : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DoomfireWarlocks();

        ~DoomfireWarlocks() override = default;

        bool configure(int numModels, bool crossbows);

    protected:

        int castingModifier() const override;

        int unbindingModifier() const override;

    private:

        Weapon m_crossBow,
                m_scimitar,
                m_crossBowMaster,
                m_scimitarMaster,
                m_steedsBite;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Doomfire Coven                   Yes
// Doomfire                         TODO
//

} // namespace DaughtersOfKhaine
