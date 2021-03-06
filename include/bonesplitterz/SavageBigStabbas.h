/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz {

    class SavageBigStabbas : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        SavageBigStabbas(Warclan warclan, int numModels, int points);

        ~SavageBigStabbas() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onFriendlyModelSlain(int numSlain, Unit* attacker, Wounds::Source source) override;

    private:

        Weapon m_gorkToof;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Da Final Fling                   Yes
// The Bigger They Are              Yes
// Savagely Enthusiastic            Yes
//

} // namespace Bonesplitterz
