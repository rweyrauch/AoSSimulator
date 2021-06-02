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

    class BloodKnights : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BloodKnights(CursedBloodline bloodline, int numModels, bool standardBearers, bool hornblowers, int points);

        ~BloodKnights() override;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon  m_templarLanceOrBlade{Weapon::Type::Melee, "Templar Lance or Blade", 1, 3, 3, 3, -1, 1},
                m_templarLanceOrBladeKastellan{Weapon::Type::Melee, "Template Lance or Blade", 1, 4, 3, 3, -1, 1},
                m_hoovesAndTeeth{Weapon::Type::Melee, "Hooves and Teeth", 1, 3, 4, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// The Hunger                       Yes
// Martial Fury                     Yes
// Riders of Ruin                   TODO
//

} //namespace Soulblight

