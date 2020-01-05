/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AVERONSTORMSIRE_H
#define AVERONSTORMSIRE_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals
{

class AveronStormsire : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 130;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    AveronStormsire();
    ~AveronStormsire() override = default;

    bool configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration);

protected:

    void onStartCombat(PlayerId player) override;
    void onRestore() override { m_shatteredFlasks = false; }

private:

    bool m_shatteredFlasks = false;

    Weapon m_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Voidstorm Scroll                 No
// Spirit Flask                     Yes
// Stormsire                        No
//

} // namespace StormcastEternals

#endif //AVERONSTORMSIRE_H