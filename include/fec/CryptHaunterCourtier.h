/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CRYPTHAUNTERCOURTIER_H
#define CRYPTHAUNTERCOURTIER_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class CryptHaunterCourtier : public FleshEaterCourts
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    CryptHaunterCourtier();
    ~CryptHaunterCourtier() override = default;

    bool configure();

protected:

private:

    Weapon m_massiveBoneClub,
        m_rancidTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Noble Blood                      No
// Chosen of the King               No
// Muster King's Chosen             No
//

} // namespace FleshEaterCourt

#endif //CRYPTHAUNTERCOURTIER_H