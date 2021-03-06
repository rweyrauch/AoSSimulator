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

    class ChaosChosen : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ChaosChosen(DamnedLegion legion, MarkOfChaos mark, int numModels, bool iconBearer, bool drummer, int points);

        ChaosChosen() = delete;

        ~ChaosChosen() override;

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int iconBearer(const Unit *unit);

    private:

        Weapon  m_greataxe{Weapon::Type::Melee, "Soul Splitter", 1, 3, 3, 3, -1, 1},
                m_greataxeChampion{Weapon::Type::Melee, "Soul Splitter", 1, 4, 3, 3, -1, 1};

        lsignal::slot m_braverySlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// Drummer                          Yes
// Mark of Chaos                    Yes
// Slaughter-leaders                TODO
// Soul Splitter                    Yes
//

} // SlavesToDarkness
