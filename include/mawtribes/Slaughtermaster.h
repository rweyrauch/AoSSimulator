/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Slaughtermaster : public MawtribesBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Slaughtermaster();

        ~Slaughtermaster() override = default;

        bool configure(Lore lore);

    protected:

        void onCastSpell(const Spell *spell, const Unit *target) override;

        void onUnboundSpell(Unit *caster, int castRoll) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_stumpBlades,
                m_bite,
                m_assortedWeapons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodgruel                       Yes
// Great Cauldron                   Yes
//    Bad Meat                      Yes
//    Troggoth Guts                 Yes
//    Spinemarrow                   Yes
//    Bonecrusher                   Yes
// Rockchomper                      TODO
//

} // namespace OgorMawtribes
