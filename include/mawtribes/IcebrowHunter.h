/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ICEBROWHUNTER_H
#define ICEBROWHUNTER_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class IcebrowHunter : public MawtribesBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    IcebrowHunter();
    ~IcebrowHunter() override = default;

    bool configure();

protected:

private:

    Weapon m_spear,
        m_crossbow,
        m_club,
        m_bite;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Icy Breath                       TODO
// Masters of Ambush                TODO
// Mighty Throw                     TODO
// Lead the Skal                    TODO
//

} // namespace OgorMawtribes

#endif //ICEBROWHUNTER_H