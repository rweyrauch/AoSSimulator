/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VAMPIRELORD_H
#define VAMPIRELORD_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class VampireLord : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VampireLord();

        ~VampireLord() override = default;

        bool configure(bool withSteed, bool withWings, bool chalice);

    protected:

        void onStartHero(PlayerId player) override;
        void onRestore() override;
        Wounds onEndCombat(PlayerId player) override;

    private:

        Weapon m_blades,
            m_hoovesAndTeeth;

        bool m_haveChaliceOfBlood = false;
        bool m_usedChaliceOfBlood = false;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Hunger                       Yes
// Chalice of Blood                 Yes
// Deathly Invocation               Yes
// Blood Feast                      TODO
//

} // namespace Death

#endif // VAMPIRELORD_H