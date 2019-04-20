/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNEFATHERONMAGMADROTH_H
#define AURICRUNEFATHERONMAGMADROTH_H

#include <Unit.h>
#include <Weapon.h>

namespace Fyreslayers
{

class AuricRunefatherOnMagmadroth : public Unit
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    AuricRunefatherOnMagmadroth();
    ~AuricRunefatherOnMagmadroth() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_throwingAxe,
        m_fyrestream,
        m_clawsAndHorns,
        m_blazingMaw,
        m_grandAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               No
// Lashing Tail                     No
// Volcanic Blood                   No
// Stare Down                       No
// Weapon-breaker                   No
// Steadfast Advance                No
//

} // namespace Fyreslayers

#endif //AURICRUNEFATHERONMAGMADROTH_H