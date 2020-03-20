/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_WARDENKING_H
#define COS_WARDENKING_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class WardenKing : public CitizenOfSigmar
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    WardenKing();
    ~WardenKing() override = default;

    bool configure();

protected:


private:

    Weapon m_runeWeapon;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Oath Stone                       TODO
// Ancestral Grudge                 TODO
//

} // namespace CitiesOfSigmar

#endif //COS_WARDENKING_H