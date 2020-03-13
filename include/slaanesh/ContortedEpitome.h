/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CONTORTEDEPITOME_H
#define CONTORTEDEPITOME_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class TheContortedEpitome : public SlaaneshBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    TheContortedEpitome();
    ~TheContortedEpitome() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;

private:

    Weapon m_ravagingClaws,
        m_coiledTentacles;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Gift of Power                    TODO
// Swallow Energy                   Yes
// Horrible Fascination             TODO
// Overwhelming Acquiescence        TODO
//

} // Slannesh

#endif //CONTORTEDEPITOME_H