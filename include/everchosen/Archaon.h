/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class Archaon : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Archaon(DamnedLegion legion, Lore lore, bool isGeneral);

        Archaon() = delete;

        ~Archaon() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onStartCombat(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        int crownOfDomination(const Unit *unit);

        Rerolls targetHitRerolls(const Weapon *weapon, const Unit *attacker) const override;

    private:

        mutable int m_slayerOfKingsSixesThisCombat = 0;

        Weapon  m_slayerOfKings{Weapon::Type::Melee, "The Slayer of Kings", 1, 4, 2, 3, -2, 3},
                m_dorgharsClaws{Weapon::Type::Melee, "Monstrous Claws", 1, 2, 2, 3, -2, RAND_D6},
                m_dorgharsTails{Weapon::Type::Melee, "Lashing Tails", 3, RAND_2D6, 4, 3, 0, 1},
                m_dorgharsHeads{Weapon::Type::Melee, "Three Heads", 3, 6, 3, 3, -1, 2};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Eye of Sheerian              Yes
// The Slayer of Kings              Yes
// The Armour of Morkar             Yes
// The Crown of Domination          Yes
// Three-headed Titan
//   Filth-spewer                   Yes
//   Skull-gorger                   Yes
//   Spell-eater                    TODO
// The Everchosen                   Yes
// Warlord Without Equal            Yes
// By My Will                       Yes
// All-seeing Dominion              TODO
//

} // namespace SlavesToDarkness
