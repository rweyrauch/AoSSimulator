/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CONTORTEDEPITOME_H
#define CONTORTEDEPITOME_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class TheContortedEpitome : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    TheContortedEpitome();
    ~TheContortedEpitome() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;

private:

    Weapon m_ravagingClaws,
        m_coiledTentacles;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Gift of Power                    No
// Swallow Energy                   Yes
// Horrible Fascination             No
// Overwhelming Acquiescence        No
//

} // Slannesh

#endif //CONTORTEDEPITOME_H