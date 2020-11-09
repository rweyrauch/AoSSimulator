/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHORNEBASE_H
#define KHORNEBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Khorne {

    enum class SlaughterHost : int {
        None = 0,
        Reapers_Of_Vengeance,
        Bloodlords,
        Goretide,
        Skullfiend_Tribe
    };

    enum class CommandTrait : int {
        None,

        // Khorne Mortals
        Arch_Slaughterer,
        Unrivalled_Battle_Lust,
        Slaughterborn,
        Hungry_For_Glory,
        Berserker_Lord,
        Violent_Urgency,

        // Khorne Bloodbound
        //Arch_Slaughterer,
        //Unrivalled_Battle_Lust,
        //Slaughterborn,
        Mark_Of_The_Cannibal,
        Bloodsworn,
        Disciple_Of_Khorne,

        // Khorne Daemons
        //Arch_Slaughterer,
        //Unrivalled_Battle_Lust,
        //Slaughterborn,
        Rage_Unchained,
        Aspect_Of_Death,
        Devastating_Blow,

        // Slaughterhost specific
        Mage_Eater, // Reapers
        Slaughterers_Thirst,    // Bloodlords
        Hew_The_Foe,    // Goretide
        Master_Decapitator  // Skullfiend
    };

    enum class Artefact : int {
        None,

        // Khorne Mortals
        Heart_Seeker,
        Collar_Of_Contempt,
        Blood_Drinker,
        Gorecleaver,
        The_Crimson_Plate,
        Blood_Rune,

        // Khorne Bloodbound
        Skull_Helm_Of_Khorne,
        Blood_Forged_Armour,
        Brazen_Rune,
        Blade_Of_Endless_Bloodshed,
        Mask_Of_The_Destroyer,
        Talisman_Of_Burning_Blood,

        // Khorne Bloodbound with Totem
        Banner_Of_Rage,
        Banner_Of_Wrath,
        Banner_Of_Blood,

        // Khorne Daemons
        Argath_The_King_Of_Blades,
        Deathdealer,
        Khartoth_The_Bloodhunger,
        Hellfire_Blade,
        Harvester_Of_Skulls,
        Foes_Bane,

        The_Crimson_Crown,
        Armour_Of_Scorn,
        Mark_Of_The_Bloodreaper,
        Collar_Of_Khorne,
        Crimson_Soulstone,
        Mark_Of_The_Slayer,

        // Slaughterhost specific
        Skullshard_Mantle,  // Reapers
        Halo_Of_Blood,  // Bloodlords
        Thronebreakers_Torc,    // Goretide
        Crowncleaver    // Skullfiend
    };

    class KhorneBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        KhorneBase() = default;

        ~KhorneBase() override = default;

        void setSlaughterHost(SlaughterHost host);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        KhorneBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    protected:

        SlaughterHost m_slaughterHost = SlaughterHost::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Locus of Fury                    Yes
// Reapers of Vengeance
//    Devour the Craven             TODO
//    Leave None Alive              TODO
//    Mage Eater                    TODO
// The Bloodlords
//    Slay the Mighty               Yes
//    First in His Sight            TODO
//    Slaughter's Thirst            TODO
// The Goretide
//    Tireless Conquerors           Yes
//    Ever Onwards                  TODO
//    Hew the Foe                   TODO
// The Skullfiend Tribe
//    Skull Hunters                 Yes
//    For the Brass Citadel         TODO
//    Master Decapitator            TODO
// Blood for the Blood God          TODO
//    Bloody Exemplar               TODO
//    Spelleater Curse              TODO
//    Murderlust                    TODO
//    Apoplectic Frenzy             TODO
//    Brass Skull Meteor            TODO
//    Relentless Fury               TODO
//    Crimson Rain                  TODO
//    Slaughter Triumphant          TODO
// Summon Daemons of Khorne         TODO
//

} // namespace Khorne

#endif // KHORNEBASE_H