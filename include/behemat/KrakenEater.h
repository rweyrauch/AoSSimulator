/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef KRAKENEATER_H
#define KRAKENEATER_H

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class KrakenEater : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KrakenEater();

        ~KrakenEater() override = default;

        bool configure();

    private:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Weapon m_debris,
            m_stomp,
            m_grip,
            m_warclub;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stuff 'em in Me Net              TODO
//

} // namespace SonsOfBehemat

#endif //KRAKENEATER_H