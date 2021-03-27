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

    class ChaosSorcererOnManticore : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosSorcererOnManticore();

        ~ChaosSorcererOnManticore() override = default;

    protected:

        void configure(Lore lore);

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_staff,
                m_fangsAndClaws,
                m_tail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oracular Visions                 TODO
// Wind of Chaos                    Yes
// Territorial Predator             Yes
// Mark of Chaos                    Yes
//

} // SlavesToDarkness
