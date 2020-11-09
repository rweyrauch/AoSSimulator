/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKANAUTIRONCLAD_H
#define ARKANAUTIRONCLAD_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class ArkanautIronclad : public KharadronBase {
    public:

        enum WeaponOption {
            Great_Sky_Cannon,
            Great_Skyhook,
            Great_Volley_Cannon
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ArkanautIronclad();

        ~ArkanautIronclad() override = default;

        bool configure(WeaponOption option, Endrinwork endrinwork);

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

        WeaponOption m_weaponOption = Great_Sky_Cannon;

        Weapon m_cannonShrapnel,
                m_cannonShell,
                m_skyhook,
                m_volleyCannon,
                m_torpedoes,
                m_carbines,
                m_boardingWeapons;

        Endrinwork m_endrinwork = Endrinwork::None;

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
// Great Skyhook                    Yes
// Great Sky Cannon                 Yes
//

} // namespace KharadronOverlords

#endif //ARKANAUTIRONCLAD_H
