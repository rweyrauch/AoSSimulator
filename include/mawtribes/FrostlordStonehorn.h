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

    class FrostlordOnStonehorn : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FrostlordOnStonehorn() = delete;

        ~FrostlordOnStonehorn() override = default;

    protected:

        FrostlordOnStonehorn(Mawtribe tribe, CommandTrait trait, Artefact artefact, bool isGeneral, MountTrait mountTrait);

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int woundModifier() const override;

    private:

        Weapon m_spear{Weapon::Type::Melee, "Frost Spear", 2, 4, 3, 3, -1, 3},
                m_kicks{Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 3, 0, 1},
                m_horns{Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3},
                m_hooves{Weapon::Type::Melee, "Crushing Hooves", 2, RAND_D6, 3, 2, -1, RAND_D3};

        MountTrait m_mountTrait = MountTrait::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Earth-shattering Charge          Yes
// Stone Skeleton                   Yes
// Bellowing Voice                  TODO
//

} // namespace OgorMawtribes

