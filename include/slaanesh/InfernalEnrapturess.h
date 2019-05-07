/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef INFERNALENRAPTURESS_H
#define INFERNALENRAPTURESS_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class InfernalEnrapturess : public SlaaneshBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    InfernalEnrapturess();
    ~InfernalEnrapturess() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onStartShooting(PlayerId player) override;

private:

    Weapon m_ravagingClaw,
        m_lyreCacophonousMelody,
        m_lyreEuphonicBlast;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Discordant Disruption            No
// Harmonic Alignment               No
// Versatile Instrument             Yes
//

} // Slannesh

#endif //INFERNALENRAPTURESS_H