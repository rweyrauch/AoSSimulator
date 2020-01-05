/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODSECRATOR_H
#define BLOODSECRATOR_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodsecrator : public KhorneBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Bloodsecrator();
    ~Bloodsecrator() override = default;

    bool configure();

protected:

private:

    Weapon m_ensorcelledAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Loathsome Sorcery                No
// Rage of Khorne                   No
//

} // namespace Khorne

#endif //BLOODSECRATOR_H