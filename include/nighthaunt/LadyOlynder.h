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

        static int ComputePoints(int numModels);

        static void Init();

        LadyOlynder();

        ~LadyOlynder() override = default;

        bool configure(Lore lore);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

        void onRestore() override;

    private:

        bool m_graveSandsOfTimeUsed = false;

        Weapon m_staff,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// Grave-sands of Time              Yes
// Lifting the Veil                 Yes
// Mortarch of Grief                TODO
// Wail of the Damned               Yes
// No Rest For the Wicked           TODO
// Grief-stricken                   Yes
//


} // namespace Nighthaunt

