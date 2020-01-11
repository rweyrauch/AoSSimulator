/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZAANGORENLIGHTENED_H
#define TZAANGORENLIGHTENED_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class TzaangorEnlightened : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 9;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    TzaangorEnlightened();
    ~TzaangorEnlightened() override = default;

    bool configure(int numModels);

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_tzeentchianSpear,
        m_tzeentchianSpearAviarch,
        m_viciousBeak;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Babbling Stream of Secrets       No
// Guided by the Past               Yes
//

} // Tzeentch

#endif //TZAANGORENLIGHTENED_H