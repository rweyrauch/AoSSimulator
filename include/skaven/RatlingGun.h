/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RATLINGGUN_H
#define RATLINGGUN_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class RatlingGun : public Skaventide
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 60;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    RatlingGun();
    ~RatlingGun() override = default;

    bool configure();

protected:

private:

    Weapon m_ratlingGun,
        m_rustyKnives;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// More-more Warplead!              No
//

} // namespace Skaven

#endif //RATLINGGUN_H