/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Gnoblars : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Gnoblars() = delete;

        ~Gnoblars() override = default;

    protected:

        Gnoblars(Mawtribe tribe, int numModels);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_sharpStuff{Weapon::Type::Missile, "Sharp Stuff", 8, 1, 4, 5, 0, 1},
                m_motleyWeapons{Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 1, 5, 5, 0, 1},
                m_motleyWeaponsBiter{Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 2, 5, 5, 0, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Screeching Horde                 Yes
// Trappers                         TODO
//

} // namespace OgorMawtribes

