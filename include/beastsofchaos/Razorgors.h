/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RAZORGORS_H
#define RAZORGORS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Razorgors : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 40;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 6;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Razorgors();
    ~Razorgors() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Rerolls chargeRerolls() const override { return RerollFailed; }
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_tusksAndHooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Uncontrollable Stampede          Yes
//

} // namespace BeastsOfChaos

#endif //RAZORGORS_H