/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class GlutosOrscollion : public SlaaneshBase {
    public:

        enum WeaponOption {
            Living_Whip,
            Shining_Aegis
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        GlutosOrscollion(Host host, Lore lore, bool isGeneral);

        ~GlutosOrscollion() override;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        int castingModifier() const override;

        int unbindingModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls chargeRerolls() const override;

        void onStartHero(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int fogOfTemptation(const Unit *attacker, const Weapon *weapon, const Unit *target);

        int aperitif(const Unit *unit);

        void onBeginRound(int battleRound) override;

        void onStartBattleshock(PlayerId player) override;

        void onEndHero(PlayerId player) override;

        Rerolls castingRerolls() const override;

        Rerolls unbindingRerolls() const override;

    private:

        int woundsTaken() const {
            return wounds() - remainingWounds();
        }

        bool m_protectedByDolece = false;

        lsignal::slot m_fogConnection,
                m_aperitifConnection;

        Weapon m_greatblade,
                m_scourge,
                m_dagger,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Grand Gourmand
//   Aperitif                       Yes
//   Starter                        Yes
//   Main Course                    Yes
//   Dessert                        Yes
//   Digestif                       Yes
// Fog of Temptation                Yes
// The Leerstave of Loth'shar       Yes
// Gorge on Excess                  TODO
// Crippling Famishment             Yes
// Companions
//  Painbringer Kyazu               Yes
//  Lashmaster Vhyssk               Yes
//  Priestess Dolece                Yes
//

} // Slannesh
