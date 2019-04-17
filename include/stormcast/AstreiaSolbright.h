/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ASTREIASOLBRIGHT_H
#define ASTREIASOLBRIGHT_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals
{

class AstreiaSolbright : public StormcastEternal
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    AstreiaSolbright();
    ~AstreiaSolbright() override = default;

    bool configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls chargeRerolls() const override;

private:

    Weapon m_aetherstave,
        m_monstrousClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cycle of the Storm               No
// Spirit Flask                     No
// Supernatural Roar                No
// Thunderous Pounce                Yes
// Lightning Pulse                  No
// Prime Electrids                  Yes
// Soul Energy of the First Host    No

} // namespace StormcastEternals

#endif //ASTREIASOLBRIGHT_H