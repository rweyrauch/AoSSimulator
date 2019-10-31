/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DEATHRIDERS_H
#define DEATHRIDERS_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class KavalosDeathriders : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    enum WeaponOption
    {
        NadiriteBladeAndShield,
        NadirateSpearAndShield
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    KavalosDeathriders();
    ~KavalosDeathriders() override = default;

    bool configure(int numModels, WeaponOption option, bool necrophoros);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    int runModifier() const override;
    int chargeModifier() const override;

private:

    bool m_necrophoros = false;

    Weapon m_blade,
        m_spear,
        m_bladeHekatos,
        m_spearHekatos,
        m_hoovesAndTeeth;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Necrophoros                      Yes
// Nadirite Weapons                 Yes
// Deathrider Wedge                 No
//

} // namespace OssiarchBonereapers

#endif //DEATHRIDERS_H
