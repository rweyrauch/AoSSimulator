/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AVALENOR_H
#define AVALENOR_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class AvalenorTheStoneheartKing : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AvalenorTheStoneheartKing();

        ~AvalenorTheStoneheartKing() override = default;

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

#endif // AVALENOR_H
