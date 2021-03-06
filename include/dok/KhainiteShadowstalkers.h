/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class KhainiteShadowstalkers : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KhainiteShadowstalkers(Temple temple, int numModels, int points);

        ~KhainiteShadowstalkers() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_cursedMissiles,
                m_assassinsBlades,
                m_umbralBlades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Shadow Leap                      TODO
// Cursed Missiles                  Yes
// Harness Shadow                   Yes
//

} // namespace DaughtersOfKhaine
