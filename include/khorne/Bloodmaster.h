/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODMASTER_H
#define BLOODMASTER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Bloodmaster : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bloodmaster();

        ~Bloodmaster() override = default;

        bool configure();

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_bladeOfBlood;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Decapitating Strike              Yes
// The Blood Must Flow              TODO
//

} // namespace Khorne

#endif //BLOODMASTER_H