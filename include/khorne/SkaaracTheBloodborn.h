/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class SkaaracTheBloodborn : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SkaaracTheBloodborn(SlaughterHost host, bool isGeneral);

        SkaaracTheBloodborn() = delete;

        ~SkaaracTheBloodborn() override;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onEndCombat(PlayerId player) override;

        void onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) override;

        int infernalIron(const Unit *caster);

        int toweringHorror(const Unit *unit);

    private:

        Weapon  m_burningBlood{Weapon::Type::Missile, "Burning Blood", 8, 1, 3, 3, 0, RAND_D6},
                m_blades{Weapon::Type::Melee, "Brutal Blades", 2, 8, 3, 3, -2, 2},
                m_hooves{Weapon::Type::Melee, "Thunderous Hooves", 1, 6, 4, 3, -1, 1};

        bool m_lifeEaterHeal = false;

        lsignal::slot m_infernalIronSlot, m_toweringHorrowSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Life-eater                       Yes
// Infernal Form                    Yes
// Towering Horror                  Yes
// Undying Hate                     TODO
// Call of the Skull Throne         Yes

} // namespace Khorne
