/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKANAUTCOMPANY_H
#define ARKANAUTCOMPANY_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class ArkanautCompany : public KharadronBase
{
public:

    enum CaptainWeapon
    {
        AetherflarePistol,
        VolleyPistol,
        PrivateerPistol
    };

    static Unit *Create(const ParameterList &parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    ArkanautCompany();

    ~ArkanautCompany() override = default;

    bool configure(int numModels, int numVolleyGuns, int numSkyhooks, int numSkypikes, CaptainWeapon option);

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    Rerolls battleshockRerolls() const override;

private:

    Weapon m_privateerPistol,
        m_volleyGun,
        m_skyhook,
        m_aetherflarePistol,
        m_volleyPistol,
        m_cutter,
        m_gunButt,
        m_skypike;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Glory-seekers                    Yes
//

} // namespace KharadronOverlords

#endif //ARKANAUTCOMPANY_H
