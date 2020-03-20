/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VAMPIRELORDONDRAGON_H
#define VAMPIRELORDONDRAGON_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class VampireLordOnZombieDragon : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        VampireLordOnZombieDragon();

        ~VampireLordOnZombieDragon() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

    private:

        Weapon m_breath,
            m_deathlance,
            m_sword,
            m_maw,
            m_claws;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Pestilential Breath              TODO
// The Hunger                       TODO
// Deathlance Charge                TODO
// Anciet Shield                    TODO
// Chalice of Blood                 TODO
// Deathly Invocation               TODO
// Blood Boil                       TODO
// Dread Knights                    TODO
//

} // namespace Death

#endif // VAMPIRELORDONDRAGON_H