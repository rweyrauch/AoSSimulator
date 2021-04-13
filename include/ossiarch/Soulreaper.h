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

    class MortisanSoulreaper : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MortisanSoulreaper(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        MortisanSoulreaper() = delete;

        ~MortisanSoulreaper() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_scythe{Weapon::Type::Melee, "Soulreaper Scythe", 2, 3, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deathly Touch                    Yes
// Soulreaper                       Yes
// Soul-blast                       TODO
//

} // namespace OssiarchBonereapers

