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
    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    KorghosKhul();
    ~KorghosKhul() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_axeOfKhorne,
        m_clawsAndFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Favoured of Khorne               Yes
// Aqshy's Bane                     Yes
// Collar of Khorne                 Yes
// Reality-splitting Axe            No
// Lord of the Goretide             No
//

} // namespace Khorne

#endif //KORGHOSKHUL_H