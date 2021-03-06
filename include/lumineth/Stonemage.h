/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class AlarithStonemage : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AlarithStonemage(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~AlarithStonemage() override = default;

    protected:

        void onStartCombat(PlayerId player) override;

    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stonemage Stance                 Yes
// Gravitic Redirection             Yes
//

} // namespace LuminethRealmLords

