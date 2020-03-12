/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VERMINLORDWARPSEER_H
#define VERMINLORDWARPSEER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class VerminlordWarpseer : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 320;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    VerminlordWarpseer();

    ~VerminlordWarpseer() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    void onStartHero(PlayerId player) override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    void onStartShooting(PlayerId player) override;
    void onWounded() override;
    void onRestore() override;

    int terrifying(const Unit* target);
    int getDamageTableIndex() const;

private:

    Weapon m_tails,
        m_glaive;

    bool m_usedOrb = false;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// The Great Manipulators           Yes
// Protection of the Horned Rat     Yes
// Scry-orb
// Terrifying                       Yes
// Dreaded Warpgale
// Forth-forth, Children of the Horned Rat!
//

} // namespace Skaven

#endif //VERMINLORDWARPSEER_H