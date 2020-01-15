/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CRYPTHORRORS_H
#define CRYPTHORRORS_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class CryptHorrors : public FleshEaterCourts
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 130*4;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    CryptHorrors();
    ~CryptHorrors() override = default;

    bool configure(int numModels);

protected:

    void onStartHero(PlayerId player) override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_clubsAndTalons,
        m_clubsAndTalonsHaunter;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Warrior Elite                    Yes
// Noble Blood                      Yes
// Chosen of the King               Yes
//

} // namespace FleshEaterCourt

#endif //CRYPTHORRORS_H