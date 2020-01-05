/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HEARTHGUARDBERZERKERS_H
#define HEARTHGUARDBERZERKERS_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers
{

class HearthguardBerzerkers : public Fyreslayer
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    enum WeaponOption
    {
        BerzerkerBroadaxe,
        FlamestrikePoleaxe
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    HearthguardBerzerkers();
    ~HearthguardBerzerkers() override = default;

    bool configure(int numModels, WeaponOption weapons);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_broadaxe,
        m_broadaxeKarl,
        m_poleaxe,
        m_poleaxeKarl,
        m_throwingAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Duty Unto Death                  Yes
// Smouldering Braziers             Yes
//

} // namespace Fyreslayers

#endif //HEARTHGUARDBERZERKERS_H