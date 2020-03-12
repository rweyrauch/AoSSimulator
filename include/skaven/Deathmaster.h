/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DEATHMASTER_H
#define DEATHMASTER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Deathmaster : public Skaventide
{
public:

    enum WeaponOption
    {
        WeepingBlades,
        FightingClaws
    };

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit *Create(const ParameterList &parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Deathmaster();

    ~Deathmaster() override = default;

    bool configure(WeaponOption option);

protected:

private:

    Weapon m_stars,
        m_blades,
        m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Hidden Killer
// Running Death
// Throwing Stars
//

} // namespace Skaven

#endif //DEATHMASTER_H