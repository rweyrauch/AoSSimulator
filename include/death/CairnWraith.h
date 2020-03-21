/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CAIRNWRAITH_H
#define CAIRNWRAITH_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class CairnWraith : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CairnWraith();

        ~CairnWraith() override = default;

        bool configure();

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_scythe;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightful Touch                  Yes
// Reaped Light Corn                TODO
// Ethereal                         TODO
//

} // namespace Death

#endif // CAIRNWRAITH_H