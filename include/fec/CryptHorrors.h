/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CRYPTHORRORS_H
#define CRYPTHORRORS_H

#include <Unit.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class CryptHorrors : public Unit
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 640;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    CryptHorrors();
    ~CryptHorrors() override = default;

    bool configure(int numModels);

    void hero(PlayerId player) override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    int damageModifier(const Weapon* weapon, const Unit* target, const Dice::RollResult& woundRolls) const override;

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