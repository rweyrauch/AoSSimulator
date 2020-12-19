/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPLINTEREDFANG_H
#define SPLINTEREDFANG_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class SplinteredFang : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SplinteredFang();

        ~SplinteredFang() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_poisonedWeapons,
                m_poisonedWeaponsLeader;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Snake Charmer                    TODO
// One Cut, One Kill                Yes
//

} // SlavesToDarkness

#endif //SPLINTEREDFANG_H