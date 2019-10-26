/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CELESTANTPRIME_H
#define CELESTANTPRIME_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals
{

class CelestantPrime : public StormcastEternal
{
public:

    static const int BASESIZE = 100; //
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 340;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    CelestantPrime();
    ~CelestantPrime() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_ghalMaraz;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cometstrike Sceptre              No
// Retribution from On High         No
// Bearer of the Warhammer          No
// Orrery of Celestial Fates        No
//

} // namespace StormcastEternals

#endif //CELESTANTPRIME_H