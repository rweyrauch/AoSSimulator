/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNEFATHERONMAGMADROTH_H
#define AURICRUNEFATHERONMAGMADROTH_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers
{

class AuricRunefatherOnMagmadroth : public Fyreslayer
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    AuricRunefatherOnMagmadroth();
    ~AuricRunefatherOnMagmadroth() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

    void onStartShooting(PlayerId player) override;
    Wounds onEndCombat(PlayerId player) override;
    Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

private:

    Weapon m_throwingAxe,
        m_fyrestream,
        m_clawsAndHorns,
        m_blazingMaw,
        m_grandAxe;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Stare Down                       TODO
// Weapon-breaker                   TODO
// Steadfast Advance                TODO
//

} // namespace Fyreslayers

#endif //AURICRUNEFATHERONMAGMADROTH_H