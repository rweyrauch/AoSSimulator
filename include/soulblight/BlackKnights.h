/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class BlackKnights : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BlackKnights(CursedBloodline bloodline, int numModels, bool standardBearers, bool hornblowers, int points);

        ~BlackKnights() override;

    protected:

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int rollChargeDistance() override;

        int standardBearerBraveryMod(const Unit *unit);

    private:

        Weapon m_barrowLance,
                m_barrowLanceKnight,
                m_hoovesAndTeeth;

        lsignal::slot m_standardSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Deathly Charge                   Yes
// Crypt Shields                    Yes
//


} //namespace Soulblight
