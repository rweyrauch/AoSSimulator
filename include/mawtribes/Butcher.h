/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BUTCHER_H
#define BUTCHER_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Butcher : public MawtribesBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    enum WeaponOption
    {
        Tenderiser,
        Cleaver
    };

    Butcher();
    ~Butcher() override = default;

    bool configure(WeaponOption weaponOption);

protected:

private:

    Weapon m_tenderizer,
        m_cleaver,
        m_bite;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bloodgruel                       No
// Voracious Maw                    No
//

} // namespace OgorMawtribes

#endif //BUTCHER_H