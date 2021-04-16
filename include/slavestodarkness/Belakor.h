/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class Belakor : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Belakor(DamnedLegion legion, MarkOfChaos mark, Lore lore, bool isGeneral);

        Belakor() = delete;

        ~Belakor() override = default;

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        void onRestore() override;

        void onWounded() override;

    private:

        size_t getDamageTableIndex() const;

        Weapon m_blade{Weapon::Type::Melee, "Blade of Shadows", 2, 8, 3, 3, -2, 2},
            m_claw{Weapon::Type::Melee, "Fell Claw", 1, 1, 3, 3, -1, 2},
            m_tail{Weapon::Type::Melee, "Spearing Tail", 3, 1, 2, 1, -3, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Dark Master                  TODO
// Weakening the Enemy              TODO
// Shadow Form                      Yes
// Enfeeble Foe                     Yes
//

} // SlavesToDarkness

