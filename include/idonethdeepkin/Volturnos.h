/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VOLTURNOS_H
#define VOLTURNOS_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class Volturnos : public IdonethDeepkinBase
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Volturnos();
    ~Volturnos() override;

    bool configure();

protected:

    void onCharged() override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;
    int crestOfTheHighKings(const Unit* target);

private:

    Weapon m_theAstraSolus,
        m_deepmareJawsTalons,
        m_deepmareTails;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deepmare Horn                    Yes
// The Astra Solus                  Yes
// The Crest of the High King       Yes
// Cealith, the High King's Shield  No
// First Among Akhelians            No
// Supreme Lord of Tides            No
//

} // namespace IdonethDeepkin

#endif // VOLTURNOS_H
