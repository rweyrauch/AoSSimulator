/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class HuskardOnStonehorn : public MawtribesBase {
    public:

        enum WeaponOption {
            Harpoon_Launcher,
            Chaintrap,
            Blood_Vulture
        };

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        HuskardOnStonehorn() = delete;

        ~HuskardOnStonehorn() override = default;

    protected:

        HuskardOnStonehorn(Mawtribe tribe, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral,
                           MountTrait mountTrait);

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onStartShooting(PlayerId player) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int woundModifier() const override;

    private:

        WeaponOption m_option = Harpoon_Launcher;

        Weapon m_harpoon{Weapon::Type::Missile, "Harpoon Launcher", 20, 1, 4, 3, 0, RAND_D3},
                m_chaintrap{Weapon::Type::Missile, "Chaintrap", 12, 1, 4, 3, 0, 3},
                m_vulture{Weapon::Type::Missile, "Blood Vulture", 30, 1, 0, 0, 0, 0},
                m_kicks{Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 4, 0, 1},
                m_horns{Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3},
                m_hooves{Weapon::Type::Melee, "Crushing Hooves", 1, RAND_D6, 3, 2, -1, RAND_D3};

        MountTrait m_mountTrait = MountTrait::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Earth-shattering Charge          Yes
// Stone Skeleton                   Yes
// Blood Vulture                    Yes
// Line-breakers                    TODO
//

} // namespace OgorMawtribes

