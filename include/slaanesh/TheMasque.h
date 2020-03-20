/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THEMASQUE_H
#define THEMASQUE_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class TheMasque : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheMasque();

        ~TheMasque() override = default;

        bool configure();

    protected:

    private:

        Weapon m_ravagingClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Staff of Masks                   TODO
// The Endless Dance                TODO
// Lithe and Swift                  Yes
// Inhuman Reflexes                 TODO
//

} // Slannesh

#endif //THEMASQUE_H