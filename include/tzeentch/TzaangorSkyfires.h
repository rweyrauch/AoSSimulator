/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZAANGORSKYFIRES_H
#define TZAANGORSKYFIRES_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class TzaangorSkyfires : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 9;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    TzaangorSkyfires();
    ~TzaangorSkyfires() override = default;

    bool configure(int numModels);

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_arrowOfFate,
        m_arrowOfFateAviarch,
        m_bowStave,
        m_viciousBeak,
        m_teethAndHorns;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Judgement from Afar              Yes
// Guided by the Future             Yes
//

} // Tzeentch

#endif //TZAANGORSKYFIRES_H