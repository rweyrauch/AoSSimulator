/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SISTEROFWATCH_H
#define SISTEROFWATCH_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class SistersOfTheWatch : public CitizenOfSigmar
{
public:

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    SistersOfTheWatch();
    ~SistersOfTheWatch() override = default;

    bool configure(int numModels);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_bow,
        m_sword,
        m_bowHighSister;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Eldritch Arrows                  Yes
// Loose Until the Last             TODO
// Quicksilver Shot                 Yes
//

} // namespace CitiesOfSigmar

#endif //SISTEROFWATCH_H