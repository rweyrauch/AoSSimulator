/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class Nagash : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Nagash();

        ~Nagash() override = default;

        bool configure();

        int castingModifier() const override;

    protected:

        void onWounded() override;

        void onRestore() override;

        int unbindingModifier() const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

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

} // namespace Death
