/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BELAKOR_H
#define BELAKOR_H

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

        bool configure();

    protected:


    private:

        Weapon m_blade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Dark Master                  TODO
// Lord of Torment                  TODO
// Shadow Form                      TODO
// Enfeeble Foe                     TODO
//

} // SlavesToDarkness

#endif //BELAKOR_H
