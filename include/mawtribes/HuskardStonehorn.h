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

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        HuskardOnStonehorn();

        ~HuskardOnStonehorn() override = default;

        bool configure(WeaponOption option, MountTrait mountTrait);

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onStartShooting(PlayerId player) override;

    private:

        WeaponOption m_option = Harpoon_Launcher;

        Weapon m_harpoon,
                m_chaintrap,
                m_vulture,
                m_kicks,
                m_horns,
                m_hooves;

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

