/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class HearthguardBerzerkers : public Fyreslayer {
    public:

        enum WeaponOption {
            Berzerker_Broadaxe,
            Flamestrike_Poleaxe
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        HearthguardBerzerkers();

        ~HearthguardBerzerkers() override = default;

        bool configure(int numModels, WeaponOption weapons);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_broadaxe,
                m_broadaxeKarl,
                m_poleaxe,
                m_poleaxeKarl,
                m_throwingAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Duty Unto Death                  Yes
// Smouldering Braziers             Yes
//

} // namespace Fyreslayers
