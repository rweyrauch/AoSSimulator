/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LEVIADON_H
#define LEVIADON_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianLeviadon : public IdonethDeepkinBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    AkhelianLeviadon();
    ~AkhelianLeviadon() override = default;

    bool configure();

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
// Abilities                    Implemented
// -------------------------------------------
// Jaws of Death                    Yes
// Void Drum                        TODO
//

} // namespace IdonethDeepkin

#endif // LEVIADON_H
