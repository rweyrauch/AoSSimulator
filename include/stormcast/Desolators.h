/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DESOLATORS_H
#define DESOLATORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Desolators : public StormcastEternal
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 220;
    static const int POINTS_MAX_UNIT_SIZE = 1320;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    Desolators();

    ~Desolators() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_stormBlast,
        m_thunderAxe,
        m_clawsAndFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Fury of the Storm                Yes
// Intolerable Damage               Yes
// Sigmarite Shields                Yes
// Storm Blast                      No
//

} // namespace StormcastEternals

#endif //DESOLATORS_H