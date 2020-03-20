/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODSECRATOR_H
#define BLOODSECRATOR_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Bloodsecrator : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bloodsecrator();

        ~Bloodsecrator() override = default;

        bool configure();

    protected:

    private:

        Weapon m_ensorcelledAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Loathsome Sorcery                TODO
// Rage of Khorne                   TODO
//

} // namespace Khorne

#endif //BLOODSECRATOR_H