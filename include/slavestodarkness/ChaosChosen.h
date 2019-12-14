/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSCHOSEN_H
#define CHAOSCHOSEN_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosChosen : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ChaosChosen();
    ~ChaosChosen() override = default;

    bool configure(int numModels, bool iconBearer, bool drummer);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;
    int runModifier() const override;
    int chargeModifier() const override;
    void onRestore() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

protected:

    bool m_iconBearer = false;
    bool m_drummer = false;

private:

    Weapon m_greataxe,
        m_greataxeChampion;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      No
// Drummer                          Yes
// Mark of Chaos                    No
// Slaughter-leaders                No
// Soul Splitter                    Yes
//

} // SlavesToDarkness

#endif //CHAOSCHOSEN_H