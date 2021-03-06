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

        static int ComputePoints(int numModels);

        static void Init();

        GothizzarHarvester();

        ~GothizzarHarvester() override = default;

        bool configure(WeaponOption option);

    protected:

        void onWounded() override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        int getDamageTableIndex() const;

        Weapon m_deathsHeadMaw,
                m_sickles,
                m_bludgeons,
                m_hoovesAndTail;

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

