/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODSISTERS_H
#define BLOODSISTERS_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class BloodSisters : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodSisters();

        ~BloodSisters() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_heartshardGlaive,
                m_heartshardGlaiveGorgai,
                m_crystalTouch,
                m_crystalTouchGorgai;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Turned to Crystal                Yes
//

} // namespace DaughtersOfKhaine

#endif //BLOODSISTERS_H