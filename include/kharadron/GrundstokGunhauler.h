/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRUNDSTOKGUNHAULER_H
#define GRUNDSTOKGUNHAULER_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class GrundstokGunhauler : public KharadronBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 0;
    static const int POINTS_PER_UNIT = 150;

    enum WeaponOption
    {
        SkyCannon,
        DrillCannon
    };

    static Unit *Create(const ParameterList &parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    GrundstokGunhauler();

    ~GrundstokGunhauler() override = default;

    bool configure(WeaponOption option);

protected:

    int moveModifier() const override;
    void onStartCombat(PlayerId player) override;
    void onStartMovement(PlayerId player) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    bool m_usedAheadFull = false;
    mutable bool m_aheadFull = false;

    Weapon m_cannonShrapnel,
        m_cannonShell,
        m_drillCannon,
        m_carbines,
        m_boardingWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ahead Full                       Yes
// Escort Vessel                    No
// Bomb Racks                       Yes
// Disengage                        No
// Fly High                         No
// Drill Cannon                     Yes
// Sky Cannon                       No
//

} // namespace KharadronOverlords

#endif //GRUNDSTOKGUNHAULER_H
