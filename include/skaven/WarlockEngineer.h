/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class WarlockEngineer : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WarlockEngineer(Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~WarlockEngineer() override = default;

    protected:

        void onRestore() override;

        void onEndCombat(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_pistol,
                m_blade;

        mutable bool m_moreMoreFailed = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// More-more Warp-energy!           Yes
// Warp Lightning                   Yes
//

} // namespace Skaven
