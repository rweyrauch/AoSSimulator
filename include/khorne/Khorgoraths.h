/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHORGORATHS_H
#define KHORGORATHS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Khorgoraths : public Unit
{
public:
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 8;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 540;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    Khorgoraths();

    ~Khorgoraths() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

private:

    Weapon m_boneTentacles,
        m_clawAndFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Horrific Predator                No
// Taker of Heads                   No
//

} // namespace Khorne

#endif //KHORGORATHS_H