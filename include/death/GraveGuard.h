/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class GraveGuard : public LegionOfNagashBase {
    public:

        enum WeaponOptions {
            Wight_Blade,
            Great_Wight_Blade,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        GraveGuard();

        ~GraveGuard() override;

        bool configure(int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int rollChargeDistance() override;

        int standardBearerBraveryMod(const Unit *unit);

    private:

        WeaponOptions m_weaponOption = Wight_Blade;

        Weapon m_wightBlade,
                m_wightBladeSeneschal,
                m_greatWightBlade,
                m_greatWightBladeSeneschal;

        lsignal::slot m_standardSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Cursed Weapons                   Yes
// Crypt Shields                    Yes
//

} //namespace Death

