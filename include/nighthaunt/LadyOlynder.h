/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class LadyOlynder : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LadyOlynder(Lore lore, bool isGeneral);

        LadyOlynder() = delete;

        ~LadyOlynder() override;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        void onRestore() override;

        int mortarchOfGrief(const Unit *unit, int roll);

    private:

        bool m_graveSandsOfTimeUsed = false;

        lsignal::slot m_mortarchOfGriefSlot;

        Weapon  m_staff{Weapon::Type::Melee, "Staff of Midnight", 2, 3, 3, 3, -2, RAND_D3},
                m_claws{Weapon::Type::Melee, "Banshee Handmaidens' Spectral Claws", 1, 6, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// Grave-sands of Time              Yes
// Lifting the Veil                 Yes
// Mortarch of Grief                Yes
// Wail of the Damned               Yes
// No Rest For the Wicked           TODO
// Grief-stricken                   Yes
//


} // namespace Nighthaunt

