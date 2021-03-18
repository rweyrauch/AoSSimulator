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

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FrostlordOnStonehorn();

        ~FrostlordOnStonehorn() override = default;

        bool configure(MountTrait mountTrait);

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_spear,
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
// Bellowing Voice                  TODO
//

} // namespace OgorMawtribes

