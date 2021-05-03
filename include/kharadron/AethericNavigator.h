/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class AethericNavigator : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AethericNavigator(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~AethericNavigator() override = default;

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_pistol,
                m_zephyrscope;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aethersight                      Yes
// Aetherstorm                      Yes
// Read the Winds                   TODO
//

} // namespace KharadronOverlords

