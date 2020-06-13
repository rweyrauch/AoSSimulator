/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNESONONMAGMADROTH_H
#define AURICRUNESONONMAGMADROTH_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunesonOnMagmadroth : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunesonOnMagmadroth();

        ~AuricRunesonOnMagmadroth() override = default;

        bool configure(MountTrait trait);

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

        void onStartShooting(PlayerId player) override;

        Wounds onEndCombat(PlayerId player) override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_throwingAxe,
                m_fyrestream,
                m_clawsAndHorns,
                m_blazingMaw,
                m_javelin,
                m_warAxe,
                m_javelinMelee;
        MountTrait m_mountTrait = MountTrait::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Vying for Glory                  Yes
// Wyrmslayer Javelins              Yes
// Molten Battering Ram             TODO
//

} // namespace Fyreslayers

#endif //AURICRUNESONONMAGMADROTH_H