/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKAETHSWILDHUNT_H
#define SKAETHSWILDHUNT_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class SkaethsWildHunt : public SylvanethBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SkaethsWildHunt();
    ~SkaethsWildHunt() override = default;

    bool configure();

private:

    Weapon m_seekerBow,
        m_javalin,
        m_javalinMelee,
        m_huntingWeapon,
        m_teethAndClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Fleet of Foot                    Yes
// Might of Kurnoth                 No
//

} // namespace Sylvaneth

#endif //SKAETHSWILDHUNT_H