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

        static int ComputePoints(int numModels);

        static void Init();

        ArkhanTheBlack(Legion legion, bool isGeneral);

        ArkhanTheBlack() = delete;

        ~ArkhanTheBlack() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int castingModifier() const override;

        int unbindingModifier() const override;

    private:

        size_t getDamageTableIndex() const;

        Weapon  m_zefetKar{Weapon::Type::Melee, "Zefet-kar", 1, 1, 3, 3, -1, RAND_D3},
                m_khenashAn{Weapon::Type::Melee, "Khenash-an", 2, 1, 4, 3, -1, RAND_D3},
                m_claws{Weapon::Type::Melee, "Ebon Claws", 1, 6, 4, 3, -2, 2},
                m_clawsAndDaggers{Weapon::Type::Melee, "Spectral Claws and Dagger", 1, 6, 5, 4, 0, 1};

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

