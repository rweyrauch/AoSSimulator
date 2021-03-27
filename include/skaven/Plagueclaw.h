/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Plagueclaw : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Plagueclaw();

        ~Plagueclaw() override = default;

    protected:

        void configure();

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_catapult,
                m_knives;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Barrage of Disease               Yes
// Hideous Death                    TODO
//

} // namespace Skaven
