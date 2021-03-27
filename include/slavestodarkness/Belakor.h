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

        Belakor();

        ~Belakor() override = default;

    protected:

        void configure(Lore lore);

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_blade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Dark Master                  TODO
// Lord of Torment                  TODO
// Shadow Form                      Yes
// Enfeeble Foe                     Yes
//

} // SlavesToDarkness

