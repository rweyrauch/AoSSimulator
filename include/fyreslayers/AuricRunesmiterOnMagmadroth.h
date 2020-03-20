/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNESMITERONMAGMADROTH_H
#define AURICRUNESMITERONMAGMADROTH_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunesmiterOnMagmadroth : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunesmiterOnMagmadroth();

        ~AuricRunesmiterOnMagmadroth() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

        void onStartShooting(PlayerId player) override;

        Wounds onEndCombat(PlayerId player) override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    private:

        Weapon m_throwingAxe,
                m_fyrestream,
                m_clawsAndHorns,
                m_blazingMaw,
                m_latchAxe,
                m_runicIron;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Grand Ritual of Awakening        TODO
// Runic Empowerment                TODO
//

} // namespace Fyreslayers

#endif //AURICRUNESMITERONMAGMADROTH_H