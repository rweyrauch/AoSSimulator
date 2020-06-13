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
        Reapers_of_Vengeance,
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
        Hungry_for_Glory,
        Berserker_Lord,
        Violent_Urgency,

        // Khorne Bloodbound
        //Arch_Slaughterer,
        //Unrivalled_Battle_Lust,
        //Slaughterborn,
        Mark_of_the_Cannibal,
        Bloodsworn,
        Disciple_of_Khorne,

        // Khorne Daemons
        //Arch_Slaughterer,
        //Unrivalled_Battle_Lust,
        //Slaughterborn,
        Rage_Unchained,
        Aspect_of_Death,
        Devastating_Blow
    };

    enum class Artefact : int {
        None,

        // Khorne Mortals
        Heart_Seeker,
        Collar_of_Contempt,
        Blood_Drinker,
        Gorecleaver,
        The_Crimson_Plate,
        Blood_Rune,

        // Khorne Bloodbound
        Skull_Helm_of_Khorne,
        Blood_Forged_Armour,
        Brazen_Rune,
        Blade_of_Endless_Bloodshed,
        Mask_of_the_Destroyer,
        Talisman_of_Burning_Blood,

        // Khorne Bloodbound with Totem
        Banner_of_Rage,
        Banner_of_Wrath,
        Banner_of_Blood,

        // Khorne Daemons
        Argath_the_King_of_Blades,
        Deathdealer,
        Khartoth_the_Bloodhunger,
        Hellfire_Blade,
        Harvester_of_Skulls,
        Foes_Bane,

        The_Crimson_Crown,
        Armour_of_Scorn,
        Mark_of_the_Bloodreaper,
        Collar_of_Khorne,
        Crimson_Soulstone,
        Mark_of_the_Slayer
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
// Devour the Craven                TODO
// Skull Hunters                    Yes
// Tireless Conquerors              Yes
// Slay the Mighty                  Yes
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