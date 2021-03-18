/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class Necromancer : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Necromancer();

        ~Necromancer() override = default;

        bool configure(Lore lore);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_staff;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Undead Minions                   TODO
// Deathly Invocation               Yes
// Vanhel's Danse Macabre           TODO
//

} // namespace Death
