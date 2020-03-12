/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VERMINLORDCORRUPTOR_H
#define VERMINLORDCORRUPTOR_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class VerminlordCorruptor : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 280;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    VerminlordCorruptor();

    ~VerminlordCorruptor() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Wounds onEndCombat(PlayerId player) override;
    void onWounded() override;
    void onRestore() override;

    int terrifying(const Unit* target);
    int getDamageTableIndex() const;

private:

    Weapon m_tails,
        m_plaguereapers;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Plaguereapers                    Yes
// Plaguemaster                     Yes
// Protection of the Horned Rat     Yes
// Terrifying                       Yes
// Dreaded Plague
// Lord of Pestilence
//

} // namespace Skaven

#endif //VERMINLORDCORRUPTOR_H