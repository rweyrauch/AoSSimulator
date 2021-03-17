/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class FjulGrimnir : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        FjulGrimnir();

        ~FjulGrimnir() override = default;

        bool configure();

    protected:

        void onStartHero(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_grandAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Grimir's Blessing                Yes
// Stare Down                       Yes
// Weapon-breaker                   Yes
// Honour Our Oaths                 TODO
//

} // namespace Fyreslayers
