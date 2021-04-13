/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class MyrmideshPainbringers : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MyrmideshPainbringers();

        ~MyrmideshPainbringers() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_scimitar,
                m_scimitarMaster;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Dance of the Wailing Blade       Yes
// Painbringer Shields              Yes
//

} // Slannesh
