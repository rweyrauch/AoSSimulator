/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSVETERANCOLDONE_H
#define SAURUSVETERANCOLDONE_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusScarVeteranOnColdOne : public SeraphonBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SaurusScarVeteranOnColdOne();
    ~SaurusScarVeteranOnColdOne() override = default;

    bool configure();

protected:

private:

    Weapon m_warpick,
        m_jaws,
        m_coldOneJaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cold Ferocity                    No
// Saurian Savagery                 No

} // namespace Seraphon

#endif //SAURUSOLDBLOODCOLDONE_H