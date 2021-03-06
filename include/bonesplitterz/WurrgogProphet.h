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

    class WurrgogProphet : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        WurrgogProphet();

        ~WurrgogProphet() override = default;

        bool configure(Lore lore);

    protected:

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onStartHero(PlayerId playerId) override;

    private:

        Weapon m_staffAndShiv,
                m_fangedMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Beast Mask                       Yes
// Prophet of Da Waaagh!            Yes
// Fists of Gork                    Yes
//

} // namespace Bonesplitterz
