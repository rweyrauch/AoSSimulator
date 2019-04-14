/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDARCANUMONTAURALON_H
#define LORDARCANUMONTAURALON_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordArcanumOnTauralon : public StormcastEternal
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 340;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    LordArcanumOnTauralon();
    ~LordArcanumOnTauralon() override = default;

    bool configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_aetherstave,
        m_hornsAndHooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Comet Trail                      No
// Cycle of the Storm               No
// Meteoric Strike                  No
// Spirit Flask                     No
// Lightning Orb                    No
// Prime Electrids                  Yes
// Swift of Wing                    No

} // namespace StormcastEternals

#endif //LORDARCANUMONTAURALON_H