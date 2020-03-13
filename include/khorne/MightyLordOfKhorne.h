/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFKHORNE_H
#define LORDOFKHORNE_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class MightyLordOfKhorne : public KhorneBase
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    MightyLordOfKhorne();
    ~MightyLordOfKhorne() override = default;

    bool configure();

protected:

private:

    Weapon m_axeOfKhorne,
        m_bloodDarkClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Collar of Khorne                 Yes
// Reality-splitting Axe            TODO
// Gorelord                         TODO
//

} // namespace Khorne

#endif //LORDOFKHORNE_H