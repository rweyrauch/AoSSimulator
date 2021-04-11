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

    class DarkoathChieftain : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DarkoathChieftain(DamnedLegion legion, CommandTrait trait, Artefact artefact, bool isGeneral);

        DarkoathChieftain() = delete;
        
        ~DarkoathChieftain() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

        void onEndCombat(PlayerId player) override;

    private:

        bool m_deathblow = false;

        Weapon  m_axe{Weapon::Type::Melee, "Warlord Axe", 1, 1, 3, 3, 0, 1},
                m_broadsword{Weapon::Type::Melee, "Cursed Broadsword", 1, 3, 4, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Berserk Charge                   Yes
// Deathblow                        Yes
// Last Gasp of Glory               TODO
//

} // SlavesToDarkness
