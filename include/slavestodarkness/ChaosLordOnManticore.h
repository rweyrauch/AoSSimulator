/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosLordOnManticore : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Blade_And_Lance,
            Flail_And_Lance,
            Blade_And_Shield,
            Flail_And_Shield,
            Blade_And_Daggerfist,
            Flail_And_Daggerfist
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosLordOnManticore(DamnedLegion legion, MarkOfChaos mark, WeaponOption weapon, CommandTrait trait, Artefact artefact, bool isGeneral);

        ChaosLordOnManticore() = delete;

        ~ChaosLordOnManticore() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        WeaponOption m_weapon = WeaponOption::Blade_And_Shield;

        Weapon  m_blade{Weapon::Type::Melee, "Daemon Blade", 1, 3, 3, 3, -1, RAND_D3},
                m_lance{Weapon::Type::Melee, "Chaos Lance", 2, 3, 3, 3, 0, 2},
                m_flail{Weapon::Type::Melee, "Chaos Flail", 2, 6, 3, 3, 0, 1},
                m_fangsAndClaws{Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2},
                m_tail{Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chaos Runeshield                 Yes
// Daemonbound                      Yes
// Chaos Lance                      Yes
// Daggerfist                       Yes
// Territorial Predator             Yes
// Iron-willed Overlord             Yes
// Mark of Chaos                    Yes
//

} // SlavesToDarkness
