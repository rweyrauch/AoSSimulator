/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ISHLAENGUARD_H
#define ISHLAENGUARD_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianIshlaenGuard : public IdonethDeepkinBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    AkhelianIshlaenGuard();
    ~AkhelianIshlaenGuard() override = default;

    bool configure(int numModels, bool standardBearers, bool musicians);

protected:

    Rerolls battleshockRerolls() const override;
    Rerolls chargeRerolls() const override;
    int toSaveModifier(const Weapon *weapon) const override;

    bool m_standardBearers = false;
    bool m_musicians = false;

private:

    Weapon m_helsabre,
        m_helsabrePrince,
        m_fangmoraFangedMaw,
        m_fangmoraLashingTail;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Biovoltaic Barrier               Yes
//

} // namespace IdonethDeepkin

#endif // ISHLAENGUARD_H
