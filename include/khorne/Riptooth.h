/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RIPTOOTH_H
#define RIPTOOTH_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Riptooth : public KhorneBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 40;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Riptooth();
    ~Riptooth() override = default;

    bool configure();

protected:

    // Unflagging Hunter
    Rerolls chargeRerolls() const override { return RerollFailed; }

private:

    Weapon m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Collar of Khorne                 Yes
// Unflagging Hunter                Yes
//

} // namespace Khorne

#endif //RIPTOOTH_H