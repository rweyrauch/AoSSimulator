/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDEXORCIST_H
#define LORDEXORCIST_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordExorcist : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    LordExorcist();
    ~LordExorcist() override = default;

    bool configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onStartShooting(PlayerId player) override;

private:

    Weapon m_stave;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Redemptor Casket                 Yes
// Purifying Blast                  Yes
//

} // namespace StormcastEternals

#endif //KNIGHTINCANTOR_H