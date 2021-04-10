/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Khorgoraths : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Khorgoraths() = delete;

        ~Khorgoraths() override = default;

    protected:

        Khorgoraths(SlaughterHost host, int numModels);

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        void onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) override;

    private:

        int m_modelsSlainAtStartOfCombat = 0;

        Weapon  m_boneTentacles{Weapon::Type::Missile, "Bone Tentacles", 6, 3, 3, 4, 0, 1},
                m_clawAndFangs{Weapon::Type::Melee, "Claws and Fangs", 1, 5, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Horrific Predator                Yes
// Taker of Heads                   Yes
//

} // namespace Khorne
