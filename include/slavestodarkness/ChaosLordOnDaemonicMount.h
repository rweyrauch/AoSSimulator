/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosLordOnDaemonicMount : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ChaosLordOnDaemonicMount(DamnedLegion legion, MarkOfChaos mark, CommandTrait trait, Artefact artefact, bool isGeneral);

        ChaosLordOnDaemonicMount() = delete;

        ~ChaosLordOnDaemonicMount() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

        void onEndCombat(PlayerId player) override;

    private:

        bool m_fueledByCarnage = false;

        Weapon  m_hammer{Weapon::Type::Melee, "Cursed Warhammer", 1, 4, 3, 3, -1, 2},
                m_hooves{Weapon::Type::Melee, "Mighty Hooves", 1, 3, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chaos Runeshield                 Yes
// Cursed Warhammer                 Yes
// Fuelled by Carnage               Yes
// The Knights of Chaos             TODO
// Mark of Chaos                    Yes
//

} // SlavesToDarkness
