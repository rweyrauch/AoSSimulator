/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDCELESTANTONDRACOTH_H
#define LORDCELESTANTONDRACOTH_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordCelestantOnDracoth : public StormcastEternal
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 7;

    enum WeaponOption
    {
        TempestosHammer,
        Thunderaxe,
        LightningHammer,
        StormstrikeGlaive,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static void Init();

    LordCelestantOnDracoth();
    ~LordCelestantOnDracoth() override = default;

    bool configure(WeaponOption weapons);

protected:

    WeaponOption m_weapon;

    int extraAttacks(const Weapon *weapon) const override;

private:

    static Weapon s_stormstrikeGlaive,
        s_lightningHammer,
        s_thunderaxe,
        s_tempestosHammer,
        s_clawsAndFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lightning Hammer                 No
// Intolerable Damage               No
// Sigmarite Thundershield          No
// Stormstrike Glaive               No
// Storm Breath                     No
// Tempestos Hammer                 Yes
// Thunderaxe                       No
// Lord of the Host                 No
//

} // namespace StormcastEternals

#endif //LORDCELESTANTONDRACOTH_H