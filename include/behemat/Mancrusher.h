/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef MANCRUSHER_H
#define MANCRUSHER_H

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class Mancrusher : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Mancrusher();

        ~Mancrusher() override = default;

        bool configure(int numModels);

    private:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

        Weapon m_eadbutt,
            m_club,
            m_kick;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Keep Up!                         TODO
// Stomping Charge                  TODO
// Stuff 'Em In Me Bag              TODO
// Timber!                          TODO
//

} // namespace SonsOfBehemat

#endif //MANCRUSHER_H