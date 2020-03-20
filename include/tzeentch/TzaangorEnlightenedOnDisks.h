/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZAANGORENLIGHTENEDONDISKS_H
#define TZAANGORENLIGHTENEDONDISKS_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class TzaangorEnlightenedOnDisks : public TzeentchBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    TzaangorEnlightenedOnDisks();
    ~TzaangorEnlightenedOnDisks() override = default;

    bool configure(int numModels);

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_tzeentchianSpear,
        m_tzeentchianSpearAviarch,
        m_viciousBeak,
        m_teethAndHorns;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Babbling Stream of Secrets       TODO
// Guided by the Past               Yes
//

} // Tzeentch

#endif //TZAANGORENLIGHTENEDONDISKS_H