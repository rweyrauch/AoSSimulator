/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CRYPTGHASTCOURTIER_H
#define CRYPTGHASTCOURTIER_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class CryptGhastCourtier : public FleshEaterCourts
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 60;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    CryptGhastCourtier();
    ~CryptGhastCourtier() override = default;

    bool configure();

protected:

private:

    Weapon m_boneClub,
        m_filthyClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Trophy Hunter                    No
// Muster Serfs                     No
//

} // namespace FleshEaterCourt

#endif //CRYPTGHASTCOURTIER_H