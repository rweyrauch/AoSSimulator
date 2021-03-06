/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class TreeRevenants : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        TreeRevenants() = delete;

        ~TreeRevenants() override = default;

        TreeRevenants(Glade glade, int numModels, bool scionGlaive, bool gladeBanners, bool waypipes, int points);

    protected:

        void onBeginTurn(int battleRound, PlayerId player) override;

        Rerolls runRerolls() const override;

        Rerolls chargeRerolls() const override;

        Rerolls battleshockRerolls() const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

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
// Abilities                    Implemented
// -------------------------------------------
// Glade Banner                     Yes
// Waypipes                         TODO
// Martial Memories                 TODO
//

} // namespace Sylvaneth
