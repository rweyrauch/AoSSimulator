/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKANAUTFRIGATE_H
#define ARKANAUTFRIGATE_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class ArkanautFrigate : public KharadronBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 200;

    enum WeaponOption
    {
        HeavySkyCannon,
        HeavySkyhook
    };

    static Unit *Create(const ParameterList &parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ArkanautFrigate();

    ~ArkanautFrigate() override = default;

    bool configure(WeaponOption option);

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;
    void onStartShooting(PlayerId player) override;
    int chargeModifier() const override;
    void onStartHero(PlayerId player) override;
    Rerolls runRerolls() const override;
    void onStartCombat(PlayerId player) override;

private:

    WeaponOption m_weaponOption = HeavySkyCannon;

    Weapon m_cannonShrapnel,
        m_cannonShell,
        m_skyhook,
        m_carbines,
        m_boardingWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Garrison                         No
// Aetheric Navigator/Endrinrigger  Yes
// Bomb Racks                       Yes
// Disengage                        No
// Fly High                         No
// Heavy Skyhook                    Yes
// Heavy Sky Cannon                 Yes
//

} // namespace KharadronOverlords

#endif //ARKANAUTFRIGATE_H
