/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_RUNELORD_H
#define COS_RUNELORD_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class Runelord : public CitizenOfSigmar
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Runelord();
    ~Runelord() override = default;

    bool configure();

protected:

    // Runes of Spellbreaking
    int unbindingModifier() const override { return 2; }

private:

    Weapon m_runeStaff,
        m_forgehammer;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Runes of Spellbreaking           Yes
// Rune Lord
//    Ancestral Shield              TODO
//    Forgefire                     TODO
//

} // namespace CitiesOfSigmar

#endif //COS_RUNELORD_H