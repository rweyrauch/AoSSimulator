/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EXALTEDDEATHBRINGER_H
#define EXALTEDDEATHBRINGER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class ExaltedDeathbringer : public KhorneBase
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    enum WeaponOption
    {
        RuinousAxeAndSkullgouger,
        BloodbiteAxeAndRunemarkedShield,
        ImpalingSpear
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ExaltedDeathbringer();
    ~ExaltedDeathbringer() override = default;

    bool configure(WeaponOption weapon);

protected:

private:

    WeaponOption m_weaponOption = RuinousAxeAndSkullgouger;

    Weapon m_ruinousAxe,
        m_bloodbiteAxe,
        m_impalingSpear;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Blooded Lieutenant               TODO
// Runemarked Shield                TODO
// Skullgouger                      TODO
// Brutal Impalement                TODO
// Brutal Command                   TODO
//

} // namespace Khorne

#endif //EXALTEDDEATHBRINGER_H