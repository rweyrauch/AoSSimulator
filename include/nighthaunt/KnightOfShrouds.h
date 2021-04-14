/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class KnightOfShrouds : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KnightOfShrouds(CommandTrait trait, Artefact artefact, bool isGeneral);

        KnightOfShrouds() = delete;

        ~KnightOfShrouds() override = default;

    protected:

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon m_sword{Weapon::Type::Melee, "Sword of Stolen Hours", 1, 4, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Stolen Hours                     Yes
// Spectral Overseer                TODO
//


} // namespace Nighthaunt

