/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Greenskinz {

    class OrrukBoarboys : public Unit {
    public:

        enum WeaponOption {
            Choppa,
            Pigstikka_Spear,
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        OrrukBoarboys();

        ~OrrukBoarboys() override = default;

        bool configure(int numModels, WeaponOption weapons, bool glyphBearer, bool horns);

    protected:

        int chargeModifier() const override;

        int braveryModifier() const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

    private:

        WeaponOption m_weaponOption = Choppa;

        Weapon m_choppa,
                m_pigstikkaSpear,
                m_warBoarsTusks,
                m_choppaBoss,
                m_pigstikkaSpearBoss;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Glyph Bearer                     Yes
// Waaagh! Horns                    Yes
// Tusker Charge                    Yes
// Tusker Shield                    Yes
//

} // namespace Greenskinz
