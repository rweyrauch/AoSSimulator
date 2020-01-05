/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORGHASTARCHAI_OB_H
#define MORGHASTARCHAI_OB_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class MorghastArchai : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 210;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 3;

    enum WeaponOptions
    {
        SpiritHalberd,
        SpiritSwords,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    MorghastArchai();
    ~MorghastArchai() override = default;

    bool configure(int numModels, WeaponOptions weapons);

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    WeaponOptions m_weaponOption = SpiritHalberd;

    Weapon m_spiritHalberd,
        m_spiritSwords;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Heralds of the Accursed One      No
// Ebon-wrought Armour              Yes
//

} //namespace OssiarchBonereapers

#endif //MORGHASTARCHAI_OB_H
