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

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Runes of Spellbreaking           Yes
// Rune Lord
//    Ancestral Shield              No
//    Forgefire                     No
//

} // namespace CitiesOfSigmar

#endif //COS_RUNELORD_H