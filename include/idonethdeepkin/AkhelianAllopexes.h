/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ALLOPEXES_H
#define ALLOPEXES_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class AkhelianAllopexes : public Unit
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 8;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 4;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 560;

    enum WeaponOption
    {
        HarpoonLauncher,
        NetLauncher,
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    AkhelianAllopexes();
    ~AkhelianAllopexes() override = default;

    bool configure(int numModels, WeaponOption weapons);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bloodthirty Predators            No
//

} // namespace IdonethDeepkin

#endif // ALLOPEXES_H