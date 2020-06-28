/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TECLIS_H
#define TECLIS_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class ArchmageTeclis : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ArchmageTeclis();

        ~ArchmageTeclis() override = default;

        bool configure();

    protected:

    private:

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace LuminethRealmLords

#endif // TECLIS_H
