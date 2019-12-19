/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KATAKROS_H
#define KATAKROS_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class OrpheonKatakros : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 20;
    static const int POINTS_PER_UNIT = 500;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    OrpheonKatakros();
    ~OrpheonKatakros() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    int woundsTaken() const
    {
        return WOUNDS - remainingWounds();
    }

    Weapon m_indaKhaat,
        m_shieldImmortis,
        m_nadiriteDagger,
        m_blades,
        m_greatblade,
        m_spiritDagger;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deadly Combination               Yes
// Mortarch of the Necropolis       No
// Nadirite Weapons                 Yes
// Endless Duty                     No
// Supreme Lord of the ...          No
// Aviarch Spymaster                No
// Gnosis Scrollbearer              No
// Prime Necrophoros                No
//

} // namespace OssiarchBonereapers

#endif //KATAKROS_H
