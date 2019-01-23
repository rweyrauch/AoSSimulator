/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDCELESTANTONSTARDRAKE_H
#define LORDCELESTANTONSTARDRAKE_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordCelestantOnStardrake : public StormcastEternal
{
public:

    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 16;

    enum WeaponOption
    {
        CelestineHammer,
        StormboundBlade,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static void Init();

    LordCelestantOnStardrake();
    ~LordCelestantOnStardrake() override = default;

    bool configure(WeaponOption weapons);

protected:

    WeaponOption m_weapons = CelestineHammer;

private:

    static Weapon s_celestineHammer,
        s_stormboundBlade,
        s_greatClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcane Lineage                   No
// Cavernous Jaws                   No
// Inescapable Vengeance            No
// Lord of the Heavens              No
// Sigmarite Thundershield          No
// Stormbound Blade                 No
// Sweeping Tail                    No
// Lord of the Celestial Host       No
//

} // namespace StormcastEternals

#endif //LORDCELESTANTONSTARDRAKE_H