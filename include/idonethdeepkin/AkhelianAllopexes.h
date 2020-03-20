/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ALLOPEXES_H
#define ALLOPEXES_H

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianAllopexes : public IdonethDeepkinBase
{
public:

    enum WeaponOption
    {
        HarpoonLauncher,
        NetLauncher,
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);

    AkhelianAllopexes();
    ~AkhelianAllopexes() override = default;

    bool configure(int numModels, WeaponOption weapons);

protected:

private:

    Weapon m_harpoonLauncher,
        m_netLauncher,
        m_hooksAndBlades,
        m_allopexBite,
        m_allopexFins;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodthirty Predators            TODO
//

} // namespace IdonethDeepkin

#endif // ALLOPEXES_H
