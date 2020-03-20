/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAMELEONSKINKS_H
#define CHAMELEONSKINKS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class ChameleonSkinks : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChameleonSkinks();

        ~ChameleonSkinks() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_dartpipe,
            m_dagger;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chameleon Ambush                 TODO
// Perfect Mimicry                  TODO
// Star-venom                       Yes
//

} // namespace Seraphon

#endif //CAMELEONSKINKS_H