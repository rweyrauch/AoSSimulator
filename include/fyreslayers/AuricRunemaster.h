/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNEMASTER_H
#define AURICRUNEMASTER_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunemaster : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunemaster();

        ~AuricRunemaster() override;

        bool configure();

    protected:

        void onStartHero(PlayerId player) override;

        void onRestore() override;

        Rerolls holySeekerToHitRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target);

        Rerolls holySeekerToWoundRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_throwingAxe,
                m_brazierStaff,
                m_runicIron;

        bool m_holySeekerToHit = false;
        bool m_holySeekerToWound = false;
        Unit *m_holySeekerTarget = nullptr;

        lsignal::slot m_holySeekerToHitSlot, m_holySeekerToWoundSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Holy Seeker                      Yes
// Volcano's Call                   TODO
//

} // namespace Fyreslayers

#endif //AURICRUNEMASTER_H