/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class KixiTakaTheDiviner : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit KixiTakaTheDiviner(bool isGeneral);

        ~KixiTakaTheDiviner() override = default;

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_starbolt{Weapon::Type::Missile, "Starbolt", 18, 2, 4, 3, -1, 1},
                m_staff{Weapon::Type::Melee, "Star-stone Staff", 1, 1, 4, 4, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Star-stone Staff                 Yes
// Herald of the Old Ones           Yes
//

} // namespace Seraphon
