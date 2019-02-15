/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PINKHORRORS_H
#define PINKHORRORS_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class PinkHorrors : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 540;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    PinkHorrors();
    ~PinkHorrors() override = default;

    bool configure(int numModels, bool iconBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int castingModifier() const override;

private:

    bool m_iconBearer = false;
    bool m_hornblower = false;

    Weapon m_magicalFlames,
        m_graspingHands,
        m_graspingHandsHorror;

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
//

} // Tzeentch

#endif //PINKHORRORS_H