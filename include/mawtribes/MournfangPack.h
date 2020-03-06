/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORNFANGPACK_H
#define MORNFANGPACK_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class MournfangPack : public MawtribesBase
{
public:

    enum WeaponOption
    {
        CullingClubOrPreyHackerAndIronfist,
        GargantHacker
    };

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 840;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    MournfangPack();
    ~MournfangPack() override = default;

    bool configure(int numModels, WeaponOption weaponOption, bool hornblower, bool bannerBearer);

protected:

private:

    WeaponOption m_option = CullingClubOrPreyHackerAndIronfist;
    bool m_hornblower = false;
    bool m_bannerBearer = false;

    Weapon m_pistol,
        m_clubOrHacker,
        m_gargantHacker,
        m_tusks;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       No
// Banner Bearer                    No
// Ironfist                         No
// Mournfang Charge                 No
//

} // namespace OgorMawtribes

#endif //MORNFANGPACK_H
