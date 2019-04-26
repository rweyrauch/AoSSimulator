/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDVERITANT_H
#define LORDVERITANT_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>
#include "PrayersOfTheStormhosts.h"

namespace StormcastEternals
{

class LordVeritant : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    LordVeritant();
    ~LordVeritant() override = default;

    bool configure(PrayersOfTheStormhost prayer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_judgementBlade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Faithful Gryph-hound             No
// Lantern of Abjuration            No
// Sanction                         No
// Bound in Service                 No
//

} // namespace StormcastEternals

#endif //LORDVERITANT_H