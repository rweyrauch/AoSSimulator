/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEBRINGERHELLFLAYER_H
#define BLADEBRINGERHELLFLAYER_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class BladebringerOnHellflayer : public SlaaneshBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BladebringerOnHellflayer();
    ~BladebringerOnHellflayer() override = default;

    bool configure();

protected:

private:

    Weapon m_flensingWhips,
        m_piercingClaws,
        m_poisonedTongues;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Soulscent                        TODO
// Acquiescence                     TODO
//

} // Slannesh

#endif //BLADEBRINGERHELLFLAYER_H