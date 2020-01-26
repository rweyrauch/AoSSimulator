/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORRSARRGUARD_H
#define MORRSARRGUARD_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianMorrsarrGuard : public IdonethDeepkinBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 170;
    static const int POINTS_MAX_UNIT_SIZE = 170*4;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    AkhelianMorrsarrGuard();
    ~AkhelianMorrsarrGuard() override = default;

    bool configure(int numModels, bool standardBearers, bool musicians);

protected:

    Rerolls battleshockRerolls() const override;
    Rerolls chargeRerolls() const override;
    void onRestore() override { m_usedBiovoltaicBlast = false; }
    void onBeginTurn(int battleRound) override;
    void onStartCombat(PlayerId player) override;
    void onCharged() override;

    bool m_standardBearers = false;
    bool m_musicians = false;

    bool m_usedBiovoltaicBlast = false;

private:

    Weapon m_voltspear,
        m_voltspearPrince,
        m_fangmoraFangedMaw,
        m_fangmoraLashingTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Biovoltaic Blast                 Yes
// Wave Riders                      Yes
//

} // namespace IdonethDeepkin

#endif // MORRSARRGUARD_H
