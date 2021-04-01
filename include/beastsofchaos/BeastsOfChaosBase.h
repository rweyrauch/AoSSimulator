/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

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
        Beastial_Cunning,           // TODO
        Indomitable_Beast,
        Apex_Predator,
        Malevolent_Despoiler,       // TODO
        Oracle_Of_The_Dark_Tongue,  // TODO
        Shadowpelt,

        // Warherd Alphabeast
        Eater_Of_Heroes,
        Rampant_Juggernaut,         // TODO
        Gorger,                     // TODO
        Gouge_Tusks,
        Roaring_Brute,              // TODO
        Rugged_Hide,

        // Thunderscorn
        Tempestuous_Tyrant,
        Magnetic_Monstrosity,       // TODO
        Father_Of_The_Storm,        // TODO
        Lightning_Fast_Monstrosity, // TODO
        Adamantine_Scales,
        Ancient_Beyond_Knowledge,

        // Fray-specific
        Dominator, // Allherd TODO
        Nomadic_Leader, // Darkwalkers TODO
        Unraveling_Aura, // Gavespawn TODO
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
        Cleaver_Of_The_Brass_Bell,
        Gilded_Horns,
        Glyph_Etched_Talisman,
        Blackened_Armour_Of_Chaos,
        Champions_Doomcloak,
        Herdstone_Shard,

        // Spoils of the Thunderscorn
        Ancestral_Azyrite_Blade,
        Lightning_Chained_Bracers,
        Thunderstrike_Lodestone,
        Horn_Of_The_Tempest,
        Tanglehorn_Familiars,
        Ruinous_Icon,

        // Fray-specific
        Blade_Of_The_Desecrator, // Allherd
        Desolate_Shard, // Darkwalkers
        Mutating_Gnarlblade // Gavespawn
    };

    enum class Lore : int {
        None,

        // Lore of the Twisted Wilds
        Viletide,
        Vicious_Stanglethorns,
        Savage_Dominion,
        Tendrils_Of_Atrophy,
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

        BeastsOfChaosBase() = delete;

        ~BeastsOfChaosBase() override = default;

        void setGreatfray(Greatfray fray);

        void setArtefact(Artefact artefact);
        void setCommandTrait(CommandTrait commandTrait);

    protected:
        BeastsOfChaosBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onEndCombat(PlayerId player) override;

        int woundModifier() const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        void onBeginRound(int battleRound) override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

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
