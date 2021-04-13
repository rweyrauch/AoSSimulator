/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class DaemonPrince : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Daemonic_Axe,
            Hellforged_Sword
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        DaemonPrince(DamnedLegion legion, MarkOfChaos mark, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral);

        DaemonPrince() = delete;

        ~DaemonPrince() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_axe{Weapon::Type::Melee, "Daemonic Axe", 1, 3, 3, 3, -2, 2},
                m_sword{Weapon::Type::Melee, "Hellforged Sword", 2, 4, 4, 3, -1, RAND_D3},
                m_talons{Weapon::Type::Melee, "Malefic Talons", 1, 3, 3, 3, 0, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bounding Charge                  Yes
// Hellforged Sword                 Yes
// Immortal Champion                Yes
// Blookslick Ground                TODO
// Arcane Influence                 Yes
// Bloated Blessings                TODO
// Revel in Agony                   TODO
//

} // SlavesToDarkness

