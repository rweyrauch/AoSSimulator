/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Skaven {

    enum class CommandTrait : int {
        None,

        // Masterclan
        Malevolent,
        Verminous_Valour,
        Savage_Overlord,
        Supreme_Manipulator,    // TODO
        Master_Of_Magic,
        Cunning,                // TODO

        // Skryre
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Masterful_Scavenger,    // TODO
        Deranged_Inventor,
        Overseer_Of_Destruction,

        // Pestilens
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Master_Of_Rot_And_Ruin, // TODO
        Architect_Of_Death,
        Diseased,

        // Verminus
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Brutal_Fury,            // TODO
        Powerful,
        Devious_Adversary,      // TODO

        // Moulder
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Moulder_Supreme,        // TODO
        Hordemaster,            // TODO
        Burly,

        // Eshin
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Unrivaled_Killer,       // TODO
        Shadowmaster,           // TODO
        Incredible_Agility,
    };

    enum class Artefact : int {
        None,

        // Masterclan
        Warpstone_Scroll,
        Suspicious_Stone,
        The_Gnawshard,
        Skavenbrew,
        Snoutgrovel_Robes,
        Staff_Of_Rightful_Supremacy,

        // Skryre
        The_Brass_Orb,
        Warpstone_Armour,
        Esoteric_Warp_Resonator,
        Skryres_Breath_Bellows,
        Vial_Of_The_Fulminator,
        Vigordust_Injector,

        // Pestilens
        Blade_Of_Corruption,
        The_Foul_Pendant,
        Brooding_Blade,
        The_Fumigatous,
        Blistrevous_The_Living_Cyst,
        Liber_Bubonicus,

        // Verminus
        Things_Bane,
        Shield_Of_Distraction,
        Screechskull_Trophies,
        Flaypelt_Cloak,
        Rustcursed_Armour,
        Warpstone_Charm,

        // Moulder
        Lash_Of_Fangs,
        Foulhide,
        Snap_Snap_Snarepole,
        Rat_Tail_Snake,
        Rabid_Crown,
        Fleshgift_Vial,

        // Eshin
        Shadow_Magnet_Trinket,
        Farskitter_Cloak,
        The_Three_Fangs,
        Warpweeper_Stars,
        The_Cube_Of_Mists,
        Gnawbomb,

    };

    enum class Lore : int {
        None,

        // Grey Seer
        Scorch,
        Splinter,
        Skitterleap,
        Plague,
        Death_Frenzy,
        Warpgale,

        // Skryre
        More_More_More_Warp_Power,
        Chain_Warp_Lightning,
        Warp_Lightning_Shield,
    };

    class Skaventide : public Unit {
    public:
        Skaventide() = default;

        ~Skaventide() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact) { m_artefact = artefact; }

    protected:
        Skaventide(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int braveryModifier() const override;

        void onCommandAbilityUsed(const CommandAbility *ability, const Unit *target) override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        bool moreMore() const;

        int woundModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int castingModifier() const override;

        int unbindingModifier() const override;

        void onStartShooting(PlayerId player) override;

        void onStartHero(PlayerId player) override;

    protected:
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
        mutable int m_usedMasterOfMagicInRound = 0;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lead From the Back               TODO
// Scurry Away                      TODO
// Overwhelming Mass                Yes
// Strength in Numbers              Yes
// Skilled Manipulators             Yes
// Prized Creations                 TODO
// Masters of Murder                TODO
// Mighty Warlords                  TODO
// Warpstone Sparks                 TODO
// Echoes of the Great Plagues      TODO
//

    void Init();

} // namespace Skaven
