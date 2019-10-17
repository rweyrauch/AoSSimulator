/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDARCANUM_H
#define LORDARCANUM_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordArcanum : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    LordArcanum();
    ~LordArcanum() override = default;

    bool configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartCombat(PlayerId player) override;
    void onRestore() override { m_shatteredFlasks = false; }

private:

    bool m_shatteredFlasks = false;

    Weapon m_aetherstave;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cycle of the Storm               No
// Spirit Flask                     Yes
// Thunderclap                      Yes
// Aetheric Manipulation            No
// Prime Electrids                  Yes

} // namespace StormcastEternals

#endif //LORDARCANUM_H