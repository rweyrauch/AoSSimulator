/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DREADSCYTHEHARRIDANS_H
#define DREADSCYTHEHARRIDANS_H

#include <Unit.h>
#include <Weapon.h>

namespace Nighthaunt
{

class DreadscytheHarridans : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DreadscytheHarridans();
    ~DreadscytheHarridans() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toSaveModifier(const Weapon* weapon) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int targetHitModifier(const Weapon* weapon, const Unit* attacker) const override;

private:

    Weapon m_scythedLimbs,
        m_scythedLimbsCrone;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Harrowing Shriek                 Yes
// Murderous Bloodlust              Yes
//

} // namespace Nighthaunt

#endif // DREADSCYTHEHARRIDANS_H
