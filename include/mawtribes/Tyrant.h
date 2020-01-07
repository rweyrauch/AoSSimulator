/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TYRANT_H
#define TYRANT_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Tyrant : public MawtribesBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    enum BigName
    {
        Deathcheater,
        Brawlerguts,
        Fateseeker,
        Longstrider,
        Giantbreaker,
        Wallcrusher
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Tyrant();
    ~Tyrant() override = default;

    bool configure(BigName bigName);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

private:

    BigName m_bigName = Deathcheater;

    Weapon m_pistols,
        m_thundermace,
        m_glaive,
        m_bite;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Beastskewer Glaive               Yes
// Big Name
//        Deathcheater              Yes
//        Brawlerguts               Yes
//        Fateseeker                Yes
//        Longstrider               Yes
//        Giantbreaker              No
//        Wallcrusher               No
// Thundermace                      Yes
// Bully of the First Degree        No
//

} // namespace OgorMawtribes

#endif //TYRANT_H