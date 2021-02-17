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

    class Alarielle : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Alarielle();

        ~Alarielle() override;

        bool configure(Lore lore);

    protected:

        void onStartHero(PlayerId player) override;

        void onStartCombat(PlayerId player) override;

        void onEndMovement(PlayerId player) override;

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

        void onCharged() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls ghyransWrathToWoundReroll(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_spearOfKurnoth,
                m_talonOfDwindling,
                m_beetleGreatAntlers;

        lsignal::slot m_ghyransWrathSlot;

        bool m_usedSoulAmphorae = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Talon of the Dwindling           Yes
// Lifebloom                        Yes
// Sweeping Blows                   Yes
// Living Battering Ram             Yes
// Soul Amphorae                    Yes
// Metamorphosis                    Partial/TODO
// Ghyran's Wrath                   Yes
//

} // namespace Sylvaneth
