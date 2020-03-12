/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VERMINLORDDECEIVER_H
#define VERMINLORDDECEIVER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class VerminlordDeceiver : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 320;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels){ return POINTS_PER_UNIT; };

    static void Init();

    VerminlordDeceiver();

    ~VerminlordDeceiver() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;
    void onWounded() override;
    void onRestore() override;

    int terrifying(const Unit* target);
    int getDamageTableIndex() const;

private:

    Weapon m_doomstar,
        m_tails,
        m_warpstiletto;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Doomstar                         Yes
// Protection of the Horned Rat     Yes
// Shrouded In Darkness             Yes
// Terrifying                       Yes
// Dreaded Skitterleap
// Lord of Assassins
//

} // namespace Skaven

#endif //VERMINLORDDECEIVER_H