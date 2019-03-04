/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKMEGABOSS_H
#define ORRUKMEGABOSS_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz
{

class OrrukMegaboss : public Ironjawz
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    OrrukMegaboss();

    ~OrrukMegaboss() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

private:

    Weapon m_bossChoppa,
        m_riptoothFist;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Go on Ladz, Get Stuck In!        No
// Strength from Victory            No
// Waaagh!                          No
//

} // namespace Ironjawz

#endif //ORRUKMEGABOSS_H