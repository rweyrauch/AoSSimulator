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
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }

    AstreiaSolbright();
    ~AstreiaSolbright() override = default;

    bool configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls chargeRerolls() const override;

    void onStartCombat(PlayerId player) override;
    Wounds onEndCombat(PlayerId player) override;
    void onRestore() override { m_shatteredFlasks = false; }

private:

    bool m_shatteredFlasks = false;

    Weapon m_aetherstave,
        m_monstrousClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cycle of the Storm               No
// Spirit Flask                     Yes
// Supernatural Roar                Yes
// Thunderous Pounce                Yes
// Lightning Pulse                  Yes
// Prime Electrids                  Yes
// Soul Energy of the First Host    No

} // namespace StormcastEternals

#endif //ASTREIASOLBRIGHT_H