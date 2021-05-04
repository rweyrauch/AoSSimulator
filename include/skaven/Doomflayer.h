/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once


#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Doomflayer : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Doomflayer();

        ~Doomflayer() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onRestore() override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_whirlingBlades,
                m_rustyKnives;

        mutable bool m_moreMoreFailed = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Whirling Death                   Yes
// More-more Whirling Death!        Yes
//

} // namespace Skaven
