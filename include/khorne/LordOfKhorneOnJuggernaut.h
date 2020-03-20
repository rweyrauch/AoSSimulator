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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    LordOfKhorneOnJuggernaut();
    ~LordOfKhorneOnJuggernaut() override = default;

    bool configure();

protected:

    void onCharged() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_wrathforgedAxe,
        m_brazenHooves;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Brass-clad Shield                TODO
// Slaugtherous Charge              Yes
// Daemonic Axe                     Yes
// Blood Stampede                   TODO
//

} // namespace Khorne

#endif //LORDOFKHORNEONJUGGERNAUT_H