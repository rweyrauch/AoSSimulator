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

    class HuskardOnThundertusk : public MawtribesBase {
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

        HuskardOnThundertusk();

        ~HuskardOnThundertusk() override = default;

        bool configure(WeaponOption option, MountTrait mountTrait, Prayer prayer);

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onStartShooting(PlayerId player) override;

    private:

        WeaponOption m_option = Harpoon_Launcher;

        Weapon m_harpoon,
                m_chaintrap,
                m_vulture,
                m_ice,
                m_kicks,
                m_tusks;

        Prayer m_prayer = Prayer::None;
        MountTrait m_mountTrait = MountTrait::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blasts of Frost-wreathed Ice     Yes
// Numbing Chill                    Yes
// Blood Vulture                    Yes
// Blizzard Speaker                 TODO
//   Winter's Endurance             TODO
//   Winter's Strength              TODO
//

} // namespace OgorMawtribes

