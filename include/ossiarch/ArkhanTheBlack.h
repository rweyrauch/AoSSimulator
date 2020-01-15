/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKHANTHEBLACK_H
#define ARKHANTHEBLACK_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class ArkhanTheBlack : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 360;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ArkhanTheBlack();
    ~ArkhanTheBlack() override = default;

    int move() const override;

    bool configure();

protected:

    void onWounded() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    int getDamageTableIndex() const;

    Weapon m_zefetKar,
        m_khenashAn,
        m_claws,
        m_clawsAndDaggers;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Feaster of Souls                 No
// Frightful Touch                  Yes
// Staff of Spirits                 No
// Mortarch of Sacrament            No
// Curse of Years                   No
// First of the Mortarchs           No
//

} // namespace OssiarchBonereapers

#endif //ARKHANTHEBLACK_H
