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

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FrostlordOnThundertusk() = delete;

        ~FrostlordOnThundertusk() override = default;

    protected:

        FrostlordOnThundertusk(Mawtribe tribe, CommandTrait trait, Artefact artefact, bool isGeneral, MountTrait mountTrait);

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onStartShooting(PlayerId player) override;

        void onStartHero(PlayerId player) override;

        int woundModifier() const override;

    private:

        Weapon m_ice{Weapon::Type::Missile, "Frost-wreathed Ice", 18, 0, 0, 0, 0, 0},
                m_spear{Weapon::Type::Melee, "Frost Spear", 2, 4, 3, 3, -1, 3},
                m_kicks{Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 3, 0, 1},
                m_tusks{Weapon::Type::Melee, "Colossal Tusks", 2, 4, 3, 2, -1, RAND_D3};

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

