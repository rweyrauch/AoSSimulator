/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRuneson : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AuricRuneson();

        ~AuricRuneson() override = default;

    protected:

        void configure();

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_throwingAxe,
                m_javelin,
                m_warAxe,
                m_javelinMelee;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Vying for Glory                  Yes
// Wyrmslayer Javelins              Yes
// Dauntless Assault                Yes
//

} // namespace Fyreslayers
