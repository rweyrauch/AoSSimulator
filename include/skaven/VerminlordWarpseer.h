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

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels);

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
// Abilities                    Implemented
// -------------------------------------------
// The Great Manipulators           Yes
// Protection of the Horned Rat     Yes
// Scry-orb                         TODO
// Terrifying                       Yes
// Dreaded Warpgale                 TODO
// Forth-forth, Children of the Horned Rat! TODO
//

} // namespace Skaven

#endif //VERMINLORDWARPSEER_H