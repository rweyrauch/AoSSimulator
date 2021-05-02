/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class KnightHeraldor : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KnightHeraldor(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~KnightHeraldor() override = default;

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_broadsword;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Onwards to Glory                 Yes
// Thunderblast                     TODO
//

} // namespace StormcastEternals
