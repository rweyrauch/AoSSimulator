/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TREEREVENANTS_H
#define TREEREVENANTS_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class TreeRevenants : public SylvanethBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    TreeRevenants();
    ~TreeRevenants() override = default;

    bool configure(int numModels, bool scionGlaive, bool gladeBanners, bool waypipes);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onBeginTurn(int battleRound) override;
    Rerolls runRerolls() const override;
    Rerolls chargeRerolls() const override;
    Rerolls battleshockRerolls() const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

protected:

    bool m_gladeBanners = false;
    bool m_waypipes = false;

    // Martial memories - one reroll per phase.
    mutable bool m_combatRerollAvailable = false;
    mutable bool m_moveRerollAvailable = false;
    mutable bool m_missileRerollAvailable = false;
    mutable bool m_battleshockRerollAvailable = false;

private:

    Weapon m_enchantedBlade,
        m_enchantedBladeScion,
        m_protectorGlaive;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Glade Banner                     Yes
// Waypipes                         No
// Martial Memories                 No
//

} // namespace Sylvaneth

#endif //TREEREVENANTS_H