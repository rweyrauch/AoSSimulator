/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFKHORNEONJUGGERNAUT_H
#define LORDOFKHORNEONJUGGERNAUT_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class LordOfKhorneOnJuggernaut : public KhorneBase
{
public:
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    LordOfKhorneOnJuggernaut();
    ~LordOfKhorneOnJuggernaut() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onCharged() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_wrathforgedAxe,
        m_brazenHooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brass-clad Shield                No
// Slaugtherous Charge              Yes
// Daemonic Axe                     Yes
// Blood Stampede                   No
//

} // namespace Khorne

#endif //LORDOFKHORNEONJUGGERNAUT_H