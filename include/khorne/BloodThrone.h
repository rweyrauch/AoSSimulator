/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODTHRONE_H
#define BLOODTHRONE_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class HeraldOfKhorneOnBloodThrone : public KhorneBase
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    HeraldOfKhorneOnBloodThrone();
    ~HeraldOfKhorneOnBloodThrone() override = default;

    bool configure();

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_bladeOfBlood,
        m_hellblades,
        m_gnashingMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// The Blood Throne                 No
// Gorefeast                        No
// Decapitating Blow                Yes
//

} // namespace Khorne

#endif //BLOODTHRONE_H