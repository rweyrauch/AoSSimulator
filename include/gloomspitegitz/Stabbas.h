/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STABBAS_H
#define STABBAS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class Stabbas : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 20;
    static const int MAX_UNIT_SIZE = 60;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 360;

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

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int toWoundModifier(const Weapon* weapon, const Unit* unit) const override;
    int runModifier() const override;
    int toSaveModifier(const Weapon* weapon) const override;
    int braveryModifier() const override;
    int targetHitModifier(const Weapon* weapon, const Unit* attacker) const override;

private:

    int m_numGongbashers = 0;
    int m_numFlagbearers = 0;
    int m_numIconbearers = 0;

    Weapon m_stabba,
        m_stabbaBoss,
        m_pokinSpear,
        m_pokinSpearBoss,
        m_barbedNet;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Backstabbing Mob                 Yes
// Moonshields                      Yes
// Netters                          Yes
//

} // namespace GloomspiteGitz

#endif //STABBAS_H