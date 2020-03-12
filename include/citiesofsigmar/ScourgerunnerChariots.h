/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SCOURGECHARIOTS_H
#define SCOURGECHARIOTS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class ScourgerunnerChariots : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 150;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    ScourgerunnerChariots();
    ~ScourgerunnerChariots() override = default;

    bool configure(int numModels);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_harpoon,
        m_crossbow,
        m_hookSpear,
        m_bite,
        m_harpoonMaster,
        m_crossbowMaster;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Lay the Beast Low                Yes
//

} // namespace CitiesOfSigmar

#endif //SCOURGECHARIOTS_H