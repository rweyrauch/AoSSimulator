/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class MelusaiIronscale : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MelusaiIronscale(Temple temple, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~MelusaiIronscale() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon m_keldrisaithShooting,
                m_keldrisaith;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood of the Oracle              Yes
// Gory Offering                    TODO
// Turned to Crystal                Yes
// Wrath of the Scathborn           Yes
//

} // namespace DaughtersOfKhaine
