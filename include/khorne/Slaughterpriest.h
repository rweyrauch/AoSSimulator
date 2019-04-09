/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAUGHTERPRIEST_H
#define SLAUGHTERPRIEST_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Slaughterpriest : public KhorneBase
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    enum WeaponOption
    {
        BloodbathedAxe,
        HackbladeAndWrathHammer
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    Slaughterpriest();
    ~Slaughterpriest() override = default;

    bool configure(WeaponOption weapon);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_bloodbathedAxe,
        m_hackblade,
        m_wrathHammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Scorn of Sorcery                 No
// Blood Boil                       No
// Blood Bind                       No
//

} // namespace Khorne

#endif //SLAUGHTERPRIEST_H