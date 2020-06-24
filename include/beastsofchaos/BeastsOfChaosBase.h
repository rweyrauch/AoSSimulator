/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BEASTSOFCHAOSBASE_H
#define BEASTSOFCHAOSBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    enum class Greatfray : int {
        None,
        Allherd,
        Darkwalkers,
        Gavespawn
    };

    enum class CommandTrait : int {
        None,

        // Brayherd Alphabeast
        Beastial_Cunning,
        Indomitable_Beast,
        Apex_Predator,
        Malevolent_Despoiler,
        Oracle_of_the_Dark_Tongue,
        Shadowpelt,

        // Warherd Alphabeast
        Eater_of_Heroes,
        Rampant_Juggernaut,
        Gorger,
        Gouge_Tusks,
        Roaring_Brute,
        Rugged_Hide,

        // Thunderscorn
        Tempestuous_Tyrant,
        Magnetic_Monstrosity,
        Father_of_the_Storm,
        Lightning_Fast_Monstrosity,
        Adamantine_Scales,
        Ancient_Beyond_Knowledge,

        // Fray-specific
        Dominator, // Allherd
        Nomadic_Leader, // Darkwalkers
        Unraveling_Aura, // Gavespawn
    };

    enum class Artefact : int {
        None,

        // Spoils of the Brayherds
        Ramhorn_Helm,
        Brayblast_Trumpet,
        The_Knowing_Eye,
        Volcanic_Axe,
        Bleating_Gnarlstaff,
        Troggoth_Hide_Cloak,

        // Spoils of the Warherds
        Cleaver_of_the_Brass_Bell,
        Gilded_Horns,
        Glyph_Etched_Talisman,
        Blackened_Armour_of_Chaos,
        Champions_Doomcloak,
        Herdstone_Shard,

        // Spoils of the Thunderscorn
        Ancestral_Azyrite_Blade,
        Lightning_Chained_Bracers,
        Thunderstrike_Lodestone,
        Horn_of_the_Tempest,
        Tanglehorn_Familiars,
        Ruinous_Icon,

        // Fray-specific
        Blade_of_the_Desecrator, // Allherd
        Desolate_Shard, // Darkwalkers
        Mutating_Gnarlblade // Gavespawn
    };

    enum class Lore : int {
        None,

        // Lore of the Twisted Wilds
        Viletide,
        Vicious_Stanglethorns,
        Savage_Dominion,
        Tendrils_of_Atrophy,
        Wild_Rampage,
        Titanic_Fury,

        // Lore of Dark Storms
        Thunderwave,
        Hailstorm,
        Sundering_Blades
    };

    class BeastsOfChaosBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        BeastsOfChaosBase() = default;

        ~BeastsOfChaosBase() override = default;

        void setGreatfray(Greatfray fray);

        void setArtefact(Artefact artefact);
        void setCommandTrait(CommandTrait commandTrait);

    protected:
        BeastsOfChaosBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Wounds onEndCombat(PlayerId player) override;

    protected:

        Greatfray m_greatfray = Greatfray::None;
        Artefact m_artefact = Artefact::None;
        CommandTrait m_commandTrait = CommandTrait::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brayherd Ambush                  TODO
// Creatures of the Storm           TODO
// Bloodgorge                       Yes
// Primordial Call                  TODO
// Allherd
//    Bestial Might                 TODO
//    Booming Roar                  TODO
//    Dominator                     TODO
// Darkwalkers
//    Shadowbeasts                  TODO
//    Nomadic Leader                TODO
//    Savage Encirclement           TODO
// Gavespawn
//    Gift of Morghur               TODO
//    Propagator of Devolution      TODO
//    Unravelling Aura              TODO
//

    void Init();

} // namespace BeastsOfChaos

#endif //BEASTSOFCHAOSBASE_H