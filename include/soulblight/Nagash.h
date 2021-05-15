/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class Nagash : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Nagash(CursedBloodline bloodline, bool isGeneral);

        ~Nagash() override = default;

        int castingModifier() const override;

    protected:

        void onWounded() override;

        void onRestore() override;

        int unbindingModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_gaze,
                m_alakanash,
                m_zefetNebtar,
                m_clawsAndDaggers;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Alakanash, the Staff of Power    TODO
// Invocation of Nagash             TODO
// Frightful Touch                  Yes
// Morikhane                        TODO
// The Nine Books of Nagash         Yes
// Hand of Dust                     TODO
// Soul Stealer                     TODO
// Supreme Lord of the Undead       TODO
//

} // namespace Soulblight