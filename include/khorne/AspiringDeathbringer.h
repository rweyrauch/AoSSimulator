/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ASPIRINGDEATHBRINGER_H
#define ASPIRINGDEATHBRINGER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class AspiringDeathbringer : public KhorneBase
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;
    static const int POINTS_PER_UNIT_WITH_GOREAXE = 100;

    enum WeaponOption
    {
        BloodaxeAndWrathhammer,
        GoreaxeAndSkullhammer,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    AspiringDeathbringer();
    ~AspiringDeathbringer() override = default;

    bool configure(WeaponOption weapon);

protected:

private:

    WeaponOption m_weaponOption = BloodaxeAndWrathhammer;

    Weapon m_bloodAxe,
        m_wrathHammer,
        m_goreaxe,
        m_skullhammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bane of Cowards                  No
// Slaughter Incarnate              No
//

} // namespace Khorne

#endif //ASPIRINGDEATHBRINGER_H