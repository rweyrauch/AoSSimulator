/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EIDOLONSEA_H
#define EIDOLONSEA_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class EidolonOfMathlannAspectOfTheSea : public IdonethDeepkinBase
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 380;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    EidolonOfMathlannAspectOfTheSea();
    ~EidolonOfMathlannAspectOfTheSea() override;

    bool configure();

protected:

    int tranquilityOfTheAbyss(const Unit* target);

private:

    Weapon m_abyssalEnergy,
        m_trident,
        m_sceptre,
        m_stormshoal;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Dormant Energies                 No
// Tranquility of the Abyss         Yes
// Cloying Sea Mists                No
// Tsunami of Terror                No
//

} // namespace IdonethDeepkin

#endif // EIDOLONSEA_H
