/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLUXMASTER_H
#define FLUXMASTER_H


#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class FluxmasterHeraldOfTzeentchOnDisc : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        FluxmasterHeraldOfTzeentchOnDisc();

        ~FluxmasterHeraldOfTzeentchOnDisc() override = default;

        bool configure();

    protected:

    private:

        Weapon m_flames,
                m_staff,
                m_dagger,
                m_teethAndHorns;

        static bool s_registered;
    };
//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Tome                      TODO
// Blue Fire of Tzeentch            TODO
//

} // namespace Tzeentch

#endif //FLUXMASTER_H
