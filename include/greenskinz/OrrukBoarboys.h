/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKBOARBOYS_H
#define ORRUKBOARBOYS_H

#include <Unit.h>
#include <Weapon.h>

namespace Greenskinz
{

class OrrukBoarboys : public Unit
{
public:

    static const int BASESIZE = 60; // x35 ovals
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    enum WeaponOption
    {
        Choppa,
        PigstikkaSpear,
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    OrrukBoarboys();
    ~OrrukBoarboys() override = default;

    bool configure(int numModels, WeaponOption weapons, bool glyphBearer, bool horns);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int chargeModifier() const override;
    int braveryModifier() const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;

private:

    bool m_waaaghHorns = false;
    bool m_glyphBearer = false;
    WeaponOption m_weapons = Choppa;

    Weapon m_choppa,
        m_pigstikkaSpear,
        m_warBoarsTusks,
        m_choppaBoss,
        m_pigstikkaSpearBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Glyph Bearer                     Yes
// Waaagh! Horns                    Yes
// Tusker Charge                    Yes
// Tusker Shield                    Yes
//

} // namespace Greenskinz

#endif // ORRUKBOARBOYS_H