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

    enum WeaponOption
    {
        BloodaxeAndWrathhammer,
        GoreaxeAndSkullhammer,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Bane of Cowards                  TODO
// Slaughter Incarnate              TODO
//

} // namespace Khorne

#endif //ASPIRINGDEATHBRINGER_H