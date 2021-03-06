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

    class SistersOfSlaughter : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SistersOfSlaughter(Temple temple, int numModels, bool sacrificialKnife, bool hornblowers, bool standardBearers, int points);

        ~SistersOfSlaughter() override = default;

    protected:

        int rollBattleshock() const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    protected:

        bool m_hasSacrificialKnife = true;

    private:

        Weapon m_sacrificialKnife,
                m_sacrificialKnifeHandmaiden,
                m_barbedWhip,
                m_barbedWhipHandmaiden;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Dance of Death                   Yes
// Bladed Bucklers                  Yes
//

} // namespace DaughtersOfKhaine
