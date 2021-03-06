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

    class GothizzarHarvester : public OssiarchBonereaperBase {
    public:

        enum WeaponOption {
            Sickles,
            Bludgeons
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GothizzarHarvester(Legion legion, WeaponOption option);

        GothizzarHarvester() = delete;

        ~GothizzarHarvester() override = default;

    protected:

        void onRestore() override;

        void onWounded() override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        size_t getDamageTableIndex() const;

        Weapon  m_deathsHeadMaw{Weapon::Type::Missile, "Death's Head Maw", 16, 4, 3, 3, -1, 1},
                m_sickles{Weapon::Type::Melee, "Soulcleaver Sickles", 1, 6, 3, 3, -2, 2},
                m_bludgeons{Weapon::Type::Melee, "Soulcrusher Bludgeons", 1, 6, 3, 3, -2, 2},
                m_hoovesAndTail{Weapon::Type::Melee, "Ossified Hooves and Tail", 2, 4, 3, 2, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bone Harvest                     TODO
// Soulcrusher Bludgeons            Yes
// Soulcleaver Sickles              Yes
//

} // namespace OssiarchBonereapers

