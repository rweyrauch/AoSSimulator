/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef BONEGRINDER_H
#define BONEGRINDER_H

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class Bonegrinder : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bonegrinder();

        ~Bonegrinder() override = default;

        bool configure();

    private:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

    private:

        Weapon m_boulder,
            m_club,
            m_stomp;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// I'll Bite Your Head Off!         TODO
// Jump Up and Down                 TODO
// Longshanks                       TODO
// Timber!                          TODO
//

} // namespace SonsOfBehemat

#endif //BONEGRINDER_H