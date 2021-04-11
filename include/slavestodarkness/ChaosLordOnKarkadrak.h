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

    class ChaosLordOnKarkadrak : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosLordOnKarkadrak(DamnedLegion legion, MarkOfChaos mark, CommandTrait trait, Artefact artefact, bool isGeneral);

        ChaosLordOnKarkadrak() = delete;

        ~ChaosLordOnKarkadrak() override = default;

    protected:

        void onCharged() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

        void onEndCombat(PlayerId player) override;

    private:

        bool m_fueledByCarnage = false;

        Weapon  m_battleAxe{Weapon::Type::Melee, "Hexed Battle-axe", 1, 5, 3, 3, 0, 2},
                m_blade{Weapon::Type::Melee, "Daemonbound Blade", 1, 3, 3, 3, -1, RAND_D3},
                m_hornsAndClaws{Weapon::Type::Melee, "Tearing Horn and Claws", 1, 3, 4, 3, -1, 2},
                m_tail{Weapon::Type::Melee, "Battering Tail", 1, 2, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brutish Rampage                  Yes
// Daemonbound                      Yes
// Fuelled by Carnage               Yes
// Rune-etched Plating              Yes
// The Knights of Chaos             TODO
//

} // SlavesToDarkness
