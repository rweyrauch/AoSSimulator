/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class MorathiHighOracleOfKhaine : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MorathiHighOracleOfKhaine(Temple temple, Lore lore, bool isGeneral);

        ~MorathiHighOracleOfKhaine() override = default;

    protected:

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int castingModifier() const override;

        int unbindingModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        int m_woundsInTurn = 0;

        Weapon m_heartrender,
                m_wings;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Commanding Presence              Yes
// One Soul, Two Bodies             TODO
// Black Horror of Ulgu             TODO
// Worship Through Bloodshed        TODO
//

} // namespace DaughtersOfKhaine
