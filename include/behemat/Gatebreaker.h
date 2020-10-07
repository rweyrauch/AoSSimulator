/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef GATEBREAKER_H
#define GATEBREAKER_H

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class Gatebreaker : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Gatebreaker();

        ~Gatebreaker() override = default;

        bool configure();

    private:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Weapon m_boulder,
            m_stomp,
            m_grip,
            m_flail;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace SonsOfBehemat

#endif //GATEBREAKER_H