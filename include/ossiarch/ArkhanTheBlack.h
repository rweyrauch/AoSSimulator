/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class ArkhanTheBlack : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ArkhanTheBlack();

        ~ArkhanTheBlack() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int castingModifier() const override;

        int unbindingModifier() const override;

    private:

        int getDamageTableIndex() const;

        Weapon m_zefetKar,
                m_khenashAn,
                m_claws,
                m_clawsAndDaggers;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Feaster of Souls                 TODO
// Frightful Touch                  Yes
// Staff of Spirits                 Partial/TODO
// Mortarch of Sacrament            TODO
// Curse of Years                   TODO
// First of the Mortarchs           TODO
//

} // namespace OssiarchBonereapers

