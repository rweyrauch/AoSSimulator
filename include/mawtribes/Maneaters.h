/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MANEATERS_H
#define MANEATERS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Maneaters : public MawtribesBase
{
public:

    enum Ability
    {
        Brawlers,
        CrackShots,
        Striders,
        Stubborn
    };

    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Maneaters();
    ~Maneaters() override = default;

    bool configure(int numModels, Ability ability);

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    bool battleshockRequired() const override;

private:

    Ability m_ability;

    Weapon m_pistolsOrStars,
        m_bashers,
        m_bite;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Been There, Done That            Yes
//   Brawlers                       Yes
//   Crack Shots                    Yes
//   Striders                       Yes
//   Stubborn                       Yes

} // namespace OgorMawtribes

#endif //MANEATERS_H
