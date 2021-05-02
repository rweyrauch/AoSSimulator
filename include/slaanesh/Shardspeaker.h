/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class ShardspeakerOfSlaanesh : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ShardspeakerOfSlaanesh();

        ~ShardspeakerOfSlaanesh() override = default;

    protected:

        void configure(Lore lore);

        void onStartHero(PlayerId player) override;

        void onCastSpell(const Spell *spell, const Unit *target) override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_staff,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mist Lurkers                     Yes
// Twisted Mirror                   TODO
// Reflection Eternal               Yes
//

} // Slannesh
