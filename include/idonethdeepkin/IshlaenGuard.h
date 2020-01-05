/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ISHLAENGUARD_H
#define ISHLAENGUARD_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianIshlaenGuard : public Unit
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 560;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    AkhelianIshlaenGuard();
    ~AkhelianIshlaenGuard() override = default;

    bool configure(int numModels, bool standardBearers, bool musicians);

protected:

    Rerolls battleshockRerolls() const override;
    Rerolls chargeRerolls() const override;
    int toSaveModifier(const Weapon *weapon) const override;

    bool m_standardBearers;
    bool m_musicians;

private:

    Weapon m_helsabre,
        m_helsabrePrince,
        m_fangmoraFangedMaw,
        m_fangmoraLashingTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Biovoltaic Barrier               Yes
//

} // namespace IdonethDeepkin

#endif // ISHLAENGUARD_H
