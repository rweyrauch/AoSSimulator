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

namespace KharadronOverlords {

    class ArkanautFrigate : public KharadronBase {
    public:

        enum WeaponOption {
            HeavySkyCannon,
            HeavySkyhook
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ArkanautFrigate();

        ~ArkanautFrigate() override = default;

        bool configure(WeaponOption option);

    protected:

        void onWounded() override;

        void onRestore() override;

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
// Abilities                    Implemented
// -------------------------------------------
// Garrison                         TODO
// Aetheric Navigator/Endrinrigger  Yes
// Bomb Racks                       Yes
// Disengage                        TODO
// Fly High                         TODO
// Heavy Skyhook                    Yes
// Heavy Sky Cannon                 Yes
//

} // namespace KharadronOverlords

#endif //ARKANAUTFRIGATE_H
