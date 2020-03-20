/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AVATAROFKHAINE_H
#define AVATAROFKHAINE_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class AvatarOfKhaine : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AvatarOfKhaine();

        ~AvatarOfKhaine() override;

        bool configure();

    protected:

        int idolOfWorship(const Unit *unit);

    private:

        Weapon m_torrentOfBurningBlood,
            m_sword;

        lsignal::slot m_idolSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Animated                         TODO
// Idol of Worship                  Yes
//

} // namespace DaughtersOfKhaine

#endif //AVATAROFKHAINE_H