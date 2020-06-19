/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKAVENTIDE_H
#define SKAVENTIDE_H

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
        Supreme_Manipulator,
        Master_of_Magic,
        Cunning,

        // Skryre
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Masterful_Scavenger,
        Deranged_Inventor,
        Overseer_of_Destruction,

        // Pestilens
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Master_of_Rot_and_Ruin,
        Architect_of_Death,
        Diseased,

        // Verminus
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Brutal_Fury,
        Powerful,
        Devious_Adversary,

        // Moulder
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Moulder_Supreme,
        Hordemaster,
        Burly,

        // Eshin
        //Malevolent,
        //Verminous_Valour,
        //Savage_Overlord,
        Unrivaled_Killer,
        Shadowmaster,
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
        Staff_of_Rightful_Supremacy,

        // Skryre
        The_Brass_Orb,
        Warpstone_Armour,
        Esoteric_Warp_Resonator,
        Skryres_Breath_Bellows,
        Vial_of_the_Fulminator,
        Vigordust_Injector,

        // Pestilens
        Blade_of_Corruption,
        The_Foul_Pendant,
        Brooding_Blade,
        The_Fumigatous,
        Blistrevous_The_Living_Cyst,
        Liber_Bubonicus,

        // Verminus
        Things_Bane,
        Shield_of_Distraction,
        Screechskull_Trophies,
        Flaypelt_Cloak,
        Rustcursed_Armour,
        Warpstone_Charm,

        // Moulder
        Lash_of_Fangs,
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
        The_Cube_of_Mists,
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
        Warp_Lighning_Shield,
    };

    class Skaventide : public Unit {
    public:
        Skaventide() = default;

        ~Skaventide() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setCommandTrait(CommandTrait trait) { m_commandTrait = trait; }
        void setArtefact(Artefact artefact) { m_artefact = artefact; }

    protected:
        Skaventide(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int braveryModifier() const override;

        bool moreMore() const;

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lead From the Back               TODO
// Scurry Away                      TODO
// Overwhelming Mass                Yes
// Strength in Numbers              Yes
// Skilled Manipulators             TODO
// Prized Creations                 TODO
// Masters of Murder                TODO
// Mighty Warlords                  TODO
// Warpstone Sparks                 TODO
// Echoes of the Great Plagues      TODO
//

    void Init();

} // namespace Skaven

#endif //SKAVENTIDE_H