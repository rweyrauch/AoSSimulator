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

        ChaosSorcererOnManticore(DamnedLegion legion, MarkOfChaos mark, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ChaosSorcererOnManticore() = delete;

        ~ChaosSorcererOnManticore() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onEndHero(PlayerId player) override;

    private:

        Weapon  m_staff{Weapon::Type::Melee, "Sorcerous Reaping Staff", 2, 3, 3, 3, -1, RAND_D3},
                m_fangsAndClaws{Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2},
                m_tail{Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oracular Visions                 Yes
// Wind of Chaos                    Yes
// Territorial Predator             Yes
// Mark of Chaos                    Yes
//

} // SlavesToDarkness
