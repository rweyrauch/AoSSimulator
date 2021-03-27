/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SlannStarmaster : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SlannStarmaster();

        ~SlannStarmaster() override = default;

    protected:

        void configure(Lore lore);

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

    private:

        Weapon m_lightning;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Vassal                    TODO
// Foresight                        Yes
// Masters of Order                 Yes
// Comet's Call                     Yes
// Gift from the Heavens            Yes
//

} // namespace Seraphon
