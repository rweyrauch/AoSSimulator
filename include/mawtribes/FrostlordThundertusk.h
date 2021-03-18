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

    class FrostlordOnThundertusk : public MawtribesBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FrostlordOnThundertusk();

        ~FrostlordOnThundertusk() override = default;

        bool configure(MountTrait mountTrait);

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_ice,
                m_spear,
                m_kicks,
                m_tusks;

        MountTrait m_mountTrait = MountTrait::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blasts of Frost-wreathed Ice     Yes
// Numbing Chill                    Yes
// Bellowing Voice                  TODO
//

} // namespace OgorMawtribes

