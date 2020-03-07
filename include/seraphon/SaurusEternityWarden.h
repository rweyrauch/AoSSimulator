/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSWARDEN_H
#define SAURUSWARDEN_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusEternityWarden : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SaurusEternityWarden();
    ~SaurusEternityWarden() override = default;

    bool configure();

protected:

private:

    Weapon m_mace,
        m_jaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cold Ferocity                    No
// Selfless Protectors              No
// Prime Guardian                   No
//

} // namespace Seraphon

#endif //SAURUSWARDEN_H