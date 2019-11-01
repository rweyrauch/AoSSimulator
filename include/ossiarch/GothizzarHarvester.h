/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GOTHIZZAR_H
#define GOTHIZZAR_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class GothizzarHarvester : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 110;

    enum WeaponOption
    {
        Sickles,
        Bludgeons
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    GothizzarHarvester();
    ~GothizzarHarvester() override = default;

    bool configure(WeaponOption option);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;

private:

    int getDamageTableIndex() const;

    Weapon m_deathsHeadMaw,
        m_sickles,
        m_bludgeons,
        m_hoovesAndTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bone Harvest                     No
// Soulcrusher Bludgeons            No
// Soulcleaver Sickles              No
//

} // namespace OssiarchBonereapers

#endif //GOTHIZZAR_H
