/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKBRUTES_H
#define ORRUKBRUTES_H

#include <ironjawz/Ironjawz.h>
#include <Weapon.h>

namespace Ironjawz
{

class OrrukBrutes : public Ironjawz
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 560;

    enum WeaponOption
    {
        TwoBruteChoppas = 0,
        JaggedGorehacka,
    };
    enum BossWeaponOption
    {
        BossChoppa,
        BossKlaw,
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter& parameter);

    static int EnumStringToInt(const std::string& enumString);

    static void Init();

    OrrukBrutes();
    ~OrrukBrutes() override = default;

    bool configure(int numModels, WeaponOption weapons, int numGoreChoppas, BossWeaponOption bossWeapon);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

protected:

    Weapon m_twoBruteChoppas,
        m_gorehacka,
        m_gorechoppa,
        m_bossChoppa,
        m_bossKlawAndBruteSmasha;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Duff Up da Big Thing             Yes
//

} // namespace Ironjawz

#endif // ORRUKBRUTES_H