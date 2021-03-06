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

    class BlackCoach : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BlackCoach(Procession procession);

        ~BlackCoach() override = default;

    protected:

        void onWounded() override;

        size_t getDamageTableIndex() const;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onBeginRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        void onCharged() override;

        void onRestore() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        int m_powerLevel = 0;

        Weapon m_graspMissile,
                m_scythe,
                m_grasp,
                m_claws,
                m_hoovesAndTeeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// Evocation of Death               Yes
//     Nimbus of Power              Yes
//     Unholy Vigour                Yes
//     Spectral Scythes             Yes
//     Insubstantial Form           Yes
//     Witch-fire                   Yes
// Reaped Like Corn                 Yes
//


} // namespace Nighthaunt

