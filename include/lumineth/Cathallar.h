/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CATHALLAR_H
#define CATHALLAR_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class ScinariCathallar : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ScinariCathallar();

        ~ScinariCathallar() override = default;

        bool configure();

    protected:

    private:

        Weapon m_touch;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Emotional Transferance           TODO
// Darkness of the Soul             TODO
//

} // namespace LuminethRealmLords

#endif // CATHALLAR_H
