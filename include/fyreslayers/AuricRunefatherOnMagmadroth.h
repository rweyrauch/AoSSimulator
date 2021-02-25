/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNEFATHERONMAGMADROTH_H
#define AURICRUNEFATHERONMAGMADROTH_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunefatherOnMagmadroth : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunefatherOnMagmadroth();

        ~AuricRunefatherOnMagmadroth() override = default;

        bool configure(MountTrait trait);

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

        void onStartShooting(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        void onStartHero(PlayerId player) override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    private:

        Weapon m_throwingAxe,
                m_fyrestream,
                m_clawsAndHorns,
                m_blazingMaw,
                m_grandAxe;
        MountTrait m_mountTrait = MountTrait::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Stare Down                       Yes
// Weapon-breaker                   Yes
// Steadfast Advance                TODO
//

} // namespace Fyreslayers

#endif //AURICRUNEFATHERONMAGMADROTH_H