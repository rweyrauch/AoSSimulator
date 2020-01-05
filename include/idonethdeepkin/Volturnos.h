/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VOLTURNOS_H
#define VOLTURNOS_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class Volturnos : public Unit
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Volturnos();
    ~Volturnos() override = default;

    bool configure();

protected:

    void onCharged() override;

private:

    Weapon m_theAstraSolus,
        m_deepmareJawsTalons,
        m_deepmareTails;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deepmare Horn                    Yes
// The Astra Solus                  No
// The Crest of the High King       No
// Cealith, the High King's Shield  No
// First Among Akhelians            No
// Supreme Lord of Tides            No
//

} // namespace IdonethDeepkin

#endif // VOLTURNOS_H
