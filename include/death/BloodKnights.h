/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class BloodKnights : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BloodKnights(int points);

        ~BloodKnights() override;

        bool configure(int numModels, bool standardBearers, bool hornblowers);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int rollChargeDistance() override;

        void onEndCombat(PlayerId player) override;

        int standardBearerBraveryMod(const Unit *unit);

    private:

        Weapon m_templarLanceOrBlade,
                m_templarLanceOrBladeKastellan,
                m_hoovesAndTeeth;

        lsignal::slot m_standardSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// The Hunger                       Yes
// Martial Fury                     Yes
// Bloodshields                     Yes
//

} //namespace Death

