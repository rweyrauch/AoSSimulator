/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LEVIADON_H
#define LEVIADON_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianLeviadon : public Unit
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 380;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    AkhelianLeviadon();
    ~AkhelianLeviadon() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_harpoonLauncher,
        m_crushingJaws,
        m_scythedFins,
        m_twinProngedSpear,
        m_razorshellHarpoons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Jaws of Death                    Yes
// Void Drum                        No
//

} // namespace IdonethDeepkin

#endif // LEVIADON_H
