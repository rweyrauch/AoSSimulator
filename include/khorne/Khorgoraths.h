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
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 8;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    Khorgoraths();

    ~Khorgoraths() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Horrific Predator                No
// Taker of Heads                   Yes
//

} // namespace Khorne

#endif //KHORGORATHS_H