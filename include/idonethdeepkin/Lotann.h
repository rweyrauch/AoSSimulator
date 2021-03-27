/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class Lotann : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Lotann();

        ~Lotann() override;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int catalogueOfSouls(const Unit *target);

    private:

        Weapon m_quill,
                m_cudgel,
                m_blade,
                m_tentacles;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Catalogue of Souls               Yes
// Writhing Tentacles               Yes
//

} // namespace IdonethDeepkin

