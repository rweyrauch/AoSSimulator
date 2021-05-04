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

    class KnightOfShroudsOnEtherealSteed : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KnightOfShroudsOnEtherealSteed(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral);

        KnightOfShroudsOnEtherealSteed() = delete;

        ~KnightOfShroudsOnEtherealSteed() override = default;

    protected:

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

    private:

        Weapon  m_sword{Weapon::Type::Melee, "Sword of Stolen Hours", 1, 4, 3, 3, -1, 2},
                m_hoovesAndTeeth{Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 2, 4, 5, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Stolen Hours                     Yes
// Lord of Gheists                  TODO
//


} // namespace Nighthaunt

