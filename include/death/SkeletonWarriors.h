/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class SkeletonWarriors : public LegionOfNagashBase {
    public:

        enum WeaponOptions {
            Ancient_Blade,
            Ancient_Spear,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SkeletonWarriors(Legion legion, int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers, int points);

        ~SkeletonWarriors() override;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int rollChargeDistance() override;

        int standardBearerBraveryMod(const Unit *unit);

    private:

        Weapon m_ancientBlade,
                m_ancientBladeChampion,
                m_ancientSpear,
                m_ancientSpearChampion;

        lsignal::slot m_standardSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Skeleton Legion                  Yes
// Crypt Shields                    Yes
//

} //namespace Death
