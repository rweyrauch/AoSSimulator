/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VERMINLORDWARBRINGER_H
#define VERMINLORDWARBRINGER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class VerminlordWarbringer : public Skaventide
{
public:

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels);

    static void Init();

    VerminlordWarbringer();

    ~VerminlordWarbringer() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    void onWounded() override;
    void onRestore() override;

    int terrifying(const Unit* target);
    int getDamageTableIndex() const;

private:

    Weapon m_tails,
        m_glaive,
        m_fist;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Amidst the Seething Tide         Yes
// Fist of Verminus Supremacy       Yes
// Protection of the Horned Rat     Yes
// Terrifying                       Yes
// Dreaded Death Frenzy             TODO
// Tyrand of Battle                 TODO
//

} // namespace Skaven

#endif //VERMINLORDWARBRINGER_H