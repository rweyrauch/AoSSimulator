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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Dormant Energies                 TODO
// Tranquility of the Abyss         Yes
// Cloying Sea Mists                TODO
// Tsunami of Terror                TODO
//

} // namespace IdonethDeepkin

#endif // EIDOLONSEA_H
