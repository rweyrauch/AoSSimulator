/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HORRORS_H
#define HORRORS_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class HorrorsOfTzeentch : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 540;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    HorrorsOfTzeentch();
    ~HorrorsOfTzeentch() override = default;

    bool configure(int numModels, bool iconBearer, bool hornblower);

protected:

    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int castingModifier() const override;

private:

    bool m_iconBearer = false;
    bool m_hornblower = false;

    Weapon m_magicalFlamesPink,
        m_magicalFlamesBlue,
        m_magicalFlamesBrimstone,
        m_talonedHandsPink,
        m_talonedHandsBlue,
        m_talonedHandsBrimstone,
        m_talonedHandsIridescent;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// Hornblower                       No
// Flickering Flames                Yes
// Locus of Conjuration             Yes
// Ectoplasmic Elasticity           No
// Split and Split Again            No
// Petty Vengeance                  No
// Channelled Pink Fire             No
//

} // Tzeentch

#endif //HORRORS_H