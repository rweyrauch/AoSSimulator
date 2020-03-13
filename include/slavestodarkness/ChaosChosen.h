/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSCHOSEN_H
#define CHAOSCHOSEN_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosChosen : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 140*4;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ChaosChosen();
    ~ChaosChosen() override;

    bool configure(int numModels, bool iconBearer, bool drummer);

protected:

    void onWounded() override;
    int runModifier() const override;
    int chargeModifier() const override;
    void onRestore() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    int iconBearer(const Unit* unit);

protected:

    bool m_iconBearer = false;
    bool m_drummer = false;

private:

    Weapon m_greataxe,
        m_greataxeChampion;

    lsignal::slot m_braverySlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// Drummer                          Yes
// Mark of Chaos                    Yes
// Slaughter-leaders                TODO
// Soul Splitter                    Yes
//

} // SlavesToDarkness

#endif //CHAOSCHOSEN_H