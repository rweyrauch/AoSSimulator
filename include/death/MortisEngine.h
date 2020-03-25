/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORTISENGINE_H
#define MORTISENGINE_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class MortisEngine : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MortisEngine();

        ~MortisEngine() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartShooting(PlayerId player) override;

        int getDamageTableIndex() const;

    private:

        Weapon m_wail,
            m_staff,
            m_etherealWeapons;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Wail of the Damned               Yes
// Frightful Touch                  Yes
// The Reliquery                    TODO
// Bound Necromancer                TODO
//

} // namespace Death

#endif // MORTISENGINE_H