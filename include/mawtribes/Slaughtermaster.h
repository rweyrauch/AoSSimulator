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

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Slaughtermaster() = delete
                ;
        ~Slaughtermaster() override;

    protected:

        Slaughtermaster(Mawtribe tribe, CommandTrait trait, Artefact artefact, bool isGeneral, Lore lore);

    protected:

        void onCastSpell(const Spell *spell, const Unit *target) override;

        void onUnboundSpell(Unit *caster, int castRoll) override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_stumpBlades{Weapon::Type::Melee, "Stump Blades", 1, RAND_2D6, 3, 3, 0, 1},
            m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1},
            m_assortedWeapons{Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 3, 5, 5, 0, 1};

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
