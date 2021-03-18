/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed {

    class Hammerers : public Dispossessed {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Hammerers();

        ~Hammerers() override = default;

        bool configure(int numModels, bool standardBearer, bool musician);

    protected:

        bool battleshockRequired() const override;

        int rollRunDistance() override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

    private:

        Weapon m_greatHammer,
                m_greatHammerKeeper;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Kingsguard                       Yes
// Musician                         Yes
// Standard Bearer                  Yes
//

} // namespace Dispossessed
