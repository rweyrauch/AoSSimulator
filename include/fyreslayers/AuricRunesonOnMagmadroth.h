/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNESONONMAGMADROTH_H
#define AURICRUNESONONMAGMADROTH_H

#include <Unit.h>
#include <Weapon.h>

namespace Fyreslayers
{

class AuricRunesonOnMagmadroth : public Unit
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    AuricRunesonOnMagmadroth();
    ~AuricRunesonOnMagmadroth() override = default;

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
        m_javelin,
        m_warAxe,
        m_javelinMelee;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               No
// Lashing Tail                     No
// Volcanic Blood                   No
// Vying for Glory                  No
// Wyrmslayer Javelins              No
// Molten Battering Ram             No
//

} // namespace Fyreslayers

#endif //AURICRUNESONONMAGMADROTH_H