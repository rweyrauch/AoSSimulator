/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAEMONPRINCE_H
#define DAEMONPRINCE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class DaemonPrince : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            DaemonicAxe,
            HellforgedSword
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        DaemonPrince();

        ~DaemonPrince() override = default;

        bool configure(WeaponOption option);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_axe,
                m_sword,
                m_talons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bounding Charge                  Yes
// Hellforged Sword                 Yes
// Immortal Champion                TODO
// Blookslick Ground                TODO
// Arcane Influence                 TODO
// Bloated Blessings                TODO
// Revel in Agony                   TODO
//

} // SlavesToDarkness

#endif //DAEMONPRINCE_H
