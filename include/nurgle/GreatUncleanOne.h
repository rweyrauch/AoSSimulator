/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle {

    class GreatUncleanOne : public NurgleBase {
    public:

        enum WeaponOptionOne {
            Plague_Flail,
            Bileblade
        };

        enum WeaponOptionTwo {
            Massive_Bilesword,
            Doomsday_Bell
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        GreatUncleanOne();

        ~GreatUncleanOne() override = default;

    protected:

        void configure(WeaponOptionOne optionOne, WeaponOptionTwo optionTwo, Lore lore);

        void onWounded() override;

        void onCharged() override;

        size_t getDamageTableIndex() const;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_bile,
                m_flail,
                m_bilesword,
                m_bileblade,
                m_doomsdayBell,
                m_nurglings;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blubber and Bile                 Yes
// Corpulent Mass                   Yes
// Mountain of Loathsome Flesh      Yes
// Putrid Offerings                 TODO
// Reverberating Summons            TODO
// Plague Wind                      TODO
//

} // Nurgle
