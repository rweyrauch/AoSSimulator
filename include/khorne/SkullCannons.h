/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLCANNONS_H
#define SKULLCANNONS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class SkullCannons : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkullCannons();

        ~SkullCannons() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_burningSkulls,
            m_hellblades,
            m_gnashingMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Burning Skulls                   Yes
// Grind their Bones, Seize their Skulls    TODO
// Decapitating Blow                Yes
//

} // namespace Khorne

#endif //SKULLCANNONS_H