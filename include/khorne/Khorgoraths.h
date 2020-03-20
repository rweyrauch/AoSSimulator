/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHORGORATHS_H
#define KHORGORATHS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Khorgoraths : public KhorneBase
{
public:

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Khorgoraths();

    ~Khorgoraths() override = default;

    bool configure(int numModels);

protected:

    void onStartCombat(PlayerId player) override;
    Wounds onEndCombat(PlayerId player) override;

private:

    int m_modelsSlainAtStartOfCombat = 0;

    Weapon m_boneTentacles,
        m_clawAndFangs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Horrific Predator                TODO
// Taker of Heads                   Yes
//

} // namespace Khorne

#endif //KHORGORATHS_H