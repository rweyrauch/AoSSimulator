/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDRELICTOR_H
#define LORDRELICTOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>
#include "PrayersOfTheStormhosts.h"

namespace StormcastEternals
{

class LordRelictor : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    LordRelictor();
    ~LordRelictor() override = default;

    bool configure(PrayersOfTheStormhost prayer);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_relicHammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Healing Storm                    Yes
// Lightning Storm                  Yes
//

} // namespace StormcastEternals

#endif //LORDRELICTOR_H