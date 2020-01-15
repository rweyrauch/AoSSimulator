/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AKHELIANKING_H
#define AKHELIANKING_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianKing : public Unit
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 240;

    enum WeaponOption
    {
        BladedPolearm,
        Greatsword,
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    AkhelianKing();
    ~AkhelianKing() override = default;

    bool configure(WeaponOption weapon);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onCharged() override;

private:

    Weapon m_bladedPolearm,
        m_greatsword,
        m_falchion,
        m_deepmareJawsTalons,
        m_deepmareTails;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deepmare Horn                    Yes
// Akhelian Paragon                 No
// Storm of Blows                   No
// Wave Rider                       Yes
// Lord of Tides                    No
//

} // namespace IdonethDeepkin

#endif // AKHELIANKING_H
