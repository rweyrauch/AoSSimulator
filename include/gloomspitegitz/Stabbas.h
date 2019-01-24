/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STABBAS_H
#define STABBAS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class Stabbas : public Unit
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 20;
    static const int MAX_UNIT_SIZE = 60;

    enum WeaponOption
    {
        Stabba,
        PokinSpear
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    Stabbas();
    ~Stabbas() override = default;

    bool configure(int numModels, WeaponOption weapons, WeaponOption bossWeapon, int numBarbedNets,
        int numGongbashers, int numFlagbearers, int numIconbearers);

protected:

    int toWoundModifier(const Weapon* weapon, const Unit* unit) const override;
    int runModifier() const override;
    int toSaveModifier(const Weapon* weapon) const override;
    int battlshockModifier() const override;

private:

    int m_numGongbashers = 0;
    int m_numFlagbearers = 0;
    int m_numIconbearers = 0;

    static Weapon s_stabba,
        s_stabbaBoss,
        s_pokinSpear,
        s_pokinSpearBoss,
        s_barbedNet;

    static bool s_registered;
};

} // namespace GloomspiteGitz

#endif //STABBAS_H