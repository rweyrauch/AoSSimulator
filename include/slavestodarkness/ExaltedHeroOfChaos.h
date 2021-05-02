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

    class ExaltedHeroOfChaos : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ExaltedHeroOfChaos(DamnedLegion legion, MarkOfChaos mark, CommandTrait trait, Artefact artefact, bool isGeneral);

        ExaltedHeroOfChaos() = delete;

        ~ExaltedHeroOfChaos() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

        void onEndCombat(PlayerId player) override;

    private:

        bool m_thriceDamnedDagger = false;

        Weapon m_blades{Weapon::Type::Melee, "Rune-etched Blades", 1, RAND_D6, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Dark Blessings                   Yes
// Glory-hungry Bladesman           Yes
// Thrice-damned Dagger             Yes
// Trail of Red Ruin                TODO
//

} // SlavesToDarkness
