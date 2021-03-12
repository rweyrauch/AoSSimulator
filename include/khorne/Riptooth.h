/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RIPTOOTH_H
#define RIPTOOTH_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Riptooth : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Riptooth();

        ~Riptooth() override = default;

        bool configure();

    protected:

        // Unflagging Hunter
        Rerolls chargeRerolls() const override { return Rerolls::Failed; }

    private:

        Weapon m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Collar of Khorne                 Yes
// Unflagging Hunter                Yes
//

} // namespace Khorne

#endif //RIPTOOTH_H