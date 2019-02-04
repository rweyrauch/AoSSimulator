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
    static const int POINTS_PER_UNIT = 220;

    enum WeaponOption
    {
        TempestosHammer,
        Thunderaxe,
        LightningHammer,
        StormstrikeGlaive,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    LordCelestantOnDracoth();
    ~LordCelestantOnDracoth() override
    {
        delete m_pStormstrikeGlaive;
    }

    bool configure(WeaponOption weapons, bool sigmariteThundershield);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    WeaponOption m_weapon;
    bool m_sigmariteThundershield = false;

protected:

    void onBeginTurn(int battleRound) override;
    void onCharged() override;
    int extraAttacks(const Weapon *weapon) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, const Dice::RollResult& saveRolls) const override;
    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits, const WoundingHits& wounds) override;
    int damageModifier(const Weapon* weapon, const Unit* target, const Dice::RollResult& woundRolls) const override;

private:

    Weapon* m_pStormstrikeGlaive = nullptr;

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
// Intolerable Damage               Yes
// Sigmarite Thundershield          Yes
// Stormstrike Glaive               Yes
// Storm Breath                     No
// Tempestos Hammer                 Yes
// Thunderaxe                       No
// Lord of the Host                 No
//

} // namespace StormcastEternals

#endif //LORDCELESTANTONDRACOTH_H