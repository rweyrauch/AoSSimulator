/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTAZYROS_H
#define KNIGHTAZYROS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class KnightAzyros : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels)  { return POINTS_PER_UNIT; }
    static void Init();

    KnightAzyros();
    ~KnightAzyros() override;

    bool configure();

protected:

    void onStartHero(PlayerId player) override;

    Rerolls illuminatorOfTheLostReroll(const Unit* attacker, const Weapon* weapon, const Unit* target);

private:

    bool m_usedLightOfSigmar = false;

    Weapon m_starblade;

    lsignal::slot m_illuminatorSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Illuminator of the Lost          Yes
// The Light of Sigmar              Yes
//

} // namespace StormcastEternals

#endif //KNIGHTAZYROS_H