/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HELLFLAYER_H
#define HELLFLAYER_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class Hellflayer : public SlaaneshBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Hellflayer();
    ~Hellflayer() override = default;

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
//

} // Slannesh

#endif //HELLFLAYER_H