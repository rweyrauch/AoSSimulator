/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KORGHOSKHUL_H
#define KORGHOSKHUL_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class KorghosKhul : public KhorneBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    KorghosKhul();
    ~KorghosKhul() override = default;

    bool configure();

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_axeOfKhorne,
        m_clawsAndFangs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Favoured of Khorne               Yes
// Aqshy's Bane                     Yes
// Collar of Khorne                 Yes
// Reality-splitting Axe            TODO
// Lord of the Goretide             TODO
//

} // namespace Khorne

#endif //KORGHOSKHUL_H