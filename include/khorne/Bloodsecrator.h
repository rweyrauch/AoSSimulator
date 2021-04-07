/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Bloodsecrator : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bloodsecrator();

        ~Bloodsecrator() override;

    protected:

        void configure();

        int rageOfKhorneAttackMod(const Unit *attacker, const Model *attackingModel, const Weapon *weapon,
                                  const Unit *target);

        Rerolls loathsomeSorcery(const Unit* caster);

    private:

        Weapon m_ensorcelledAxe;

        lsignal::slot m_rageOfKhorneSlot, m_loathsomeSorcerySlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Loathsome Sorcery                Yes
// Rage of Khorne                   Yes
//

} // namespace Khorne
