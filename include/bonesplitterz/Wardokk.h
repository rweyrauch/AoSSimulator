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

    class Wardokk : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Wardokk();

        ~Wardokk() override = default;

    protected:

        void configure(Lore lore);

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_bonebeastStikk;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ritual Dance                     Yes
//     Grimdokk                     Yes
//     Glyphdokk                    Yes
//     Weirddokk                    Yes
//

} // namespace Bonesplitterz
