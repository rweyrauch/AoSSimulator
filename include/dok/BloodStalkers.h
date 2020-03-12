/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODSTALKERS_H
#define BLOODSTALKERS_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class BloodStalkers : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 640;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BloodStalkers();
    ~BloodStalkers() override = default;

    bool configure(int numModels, bool bloodWyrm);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_hearseekerBow,
        m_sacraficialDaggar,
        m_heartseekBowKrone,
        m_sacraficialDaggarKrone,
        m_bloodWyrm;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Heartseekers                     Yes
//

} // namespace DaughtersOfKhaine

#endif //BLOODSTALKERS_H