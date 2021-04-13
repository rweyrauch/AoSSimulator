/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>

namespace Fyreslayers {

    class Magmadroth : public Fyreslayer {
    public:

        Magmadroth() = delete;

        ~Magmadroth() override = default;

    protected:
        Magmadroth(const std::string &name, int bravery);

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onStartShooting(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        void onCharged() override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

    protected:

        Weapon m_fyrestream,
                m_clawsAndHorns,
                m_blazingMaw;

        MountTrait m_mountTrait = MountTrait::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
//

} // namespace Fyreslayers
