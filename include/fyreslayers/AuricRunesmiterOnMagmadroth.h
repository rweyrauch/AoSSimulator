/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNESMITERONMAGMADROTH_H
#define AURICRUNESMITERONMAGMADROTH_H

#include <Unit.h>
#include <Weapon.h>

namespace Fyreslayers
{

class AuricRunesmiterOnMagmadroth : public Unit
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    AuricRunesmiterOnMagmadroth();
    ~AuricRunesmiterOnMagmadroth() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    void onStartShooting(PlayerId player) override;
    void onEndCombat(PlayerId player) override;
    Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

private:

    Weapon m_throwingAxe,
        m_fyrestream,
        m_clawsAndHorns,
        m_blazingMaw,
        m_latchAxe,
        m_runicIron;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Grand Ritual of Awakening        No
// Runic Empowerment                No
//

} // namespace Fyreslayers

#endif //AURICRUNESMITERONMAGMADROTH_H