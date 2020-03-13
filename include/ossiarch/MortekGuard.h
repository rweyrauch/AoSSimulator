/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORTEKGUARD_H
#define MORTEKGUARD_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class MortekGuard : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 440;

    enum WeaponOption
    {
        NadiriteBladeAndShield,
        NadirateSpearAndShield
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    MortekGuard();
    ~MortekGuard() override = default;

    bool configure(int numModels, WeaponOption option, int numGreatblades, bool necrophoros);

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    int runModifier() const override;
    int chargeModifier() const override;

private:

    bool m_necrophoros = false;

    Weapon m_blade,
        m_spear,
        m_greatblade,
        m_bladeHekatos,
        m_spearHekatos,
        m_greatbladeHekatos;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Necrophoros                      Yes
// Nadirite Weapons                 Yes
// Shieldwall                       TODO
//

} // namespace OssiarchBonereapers

#endif //MORTEKGUARD_H
