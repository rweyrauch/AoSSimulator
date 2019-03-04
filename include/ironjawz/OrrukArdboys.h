/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKARKBOYS_H
#define ORRUKARKBOYS_H

#include <ironjawz/Ironjawz.h>
#include <Weapon.h>

namespace Ironjawz
{

class OrrukArdboys : public Ironjawz
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    enum WeaponOption
    {
        ChoppaOrSmashaWithShield = 0,
        ChoppaAndSmasha,
        BigChoppa
    };

    enum StandardOption
    {
        None = 0,
        OrrukBanner,
        IconOfGork
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter& parameter);

    static int EnumStringToInt(const std::string& enumString);

    static void Init();

    OrrukArdboys();
    ~OrrukArdboys() override = default;

    bool configure(int numModels, int numChoppasAndShield, int numPairedChoppas,
        int numBigChoppas, WeaponOption bossWeapon, bool drummer, StandardOption standard);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

    int bravery() const override;

protected:

    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    int chargeModifier() const override;
    Wounds applyWoundSave(const Wounds &wounds) override;

protected:

    Weapon m_choppaOrSmasha,
        m_choppaAndSmasha,
        m_bigChoppa,
        m_bossChoppaOrSmasha,
        m_bossChoppaAndSmasha,
        m_bossBigChoppa;

    bool m_drummer = false;
    StandardOption m_standardBearer = None;
    int m_numPairedChoppas = 0;
    int m_numChoppasAndShields = 0;
    int m_numBigChoppas = 0;
    WeaponOption m_bossWeapon;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Orruk Banner                     Yes
// Icon of Gork                     Yes
// Orruk-forged Shields             Yes
//

} // namespace Ironjawz

#endif // ORRUKARKBOYS_H