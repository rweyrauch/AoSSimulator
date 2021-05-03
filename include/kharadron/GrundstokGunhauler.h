/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class GrundstokGunhauler : public KharadronBase {
    public:

        enum WeaponOption {
            Sky_Cannon,
            Drill_Cannon
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GrundstokGunhauler(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, WeaponOption option, Endrinwork endrinwork);

        ~GrundstokGunhauler() override = default;

    protected:

        int moveModifier() const override;

        void onStartCombat(PlayerId player) override;

        void onStartMovement(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

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
// Abilities                    Implemented
// -------------------------------------------
// Ahead Full                       Yes
// Escort Vessel                    TODO
// Bomb Racks                       Yes
// Disengage                        TODO
// Can_Fly High                         TODO
// Drill Cannon                     Yes
// Sky Cannon                       TODO
//

} // namespace KharadronOverlords

