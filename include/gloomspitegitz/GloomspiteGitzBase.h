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
#include <gloomspitegitz/BadMoon.h>
#include <array>

class CommandAbility;

namespace GloomspiteGitz {

    enum class Allegiance : int {
        None,

        Jaws_of_Mork,
        Gloggs_Megamob,
        Grimscuttle_Tribes
    };

    //
    // Command Traits
    //
    enum class CommandTrait : int {
        None,

        // Blessings Of The Bad Moon
        Cunning_Plans,
        Fight_Another_Day,          // TODO
        Sneaky_Stabba,
        Tough_N_Leathery,
        Dead_Shouty,                // TODO
        The_Clammy_Hand,            // TODO

        // Gifts Of The Gloomspite
        Low_Cunning,
        Spiteful_Git,               // TODO
        Great_Shaman,
        Dodgy_Character,            // TODO
        Boss_Shaman,
        Loon_Touched,

        // Marks Of The Spider Gods Favour
        Monstrous_Mount,
        Master_Spider_Rider,
        Ulutating_Battle_Cry,
        //ToughNLeathery,
        //DeadShouty,
        Creeping_Assault,           // TODO

        // Fortuitous Troggboss Traits
        Tough_As_Rocks,
        Alpha_Trogg,
        Loonskin,
        Pulverising_Grip,
        Mighty_Blow,                // TODO
        Realmstone_Studded_Hide,

        // Allegiance
        Envoy_of_the_Overbounder,   // TODO
        Shepherd_of_Idiotic_Destruction, // TODO
        Prophet_of_da_Spider_God,   // TODO
    };

    //
    // Artefact
    //
    enum class Artefact : int {
        None,

        // Troglodytic Treasures
        Spiteful_Prodder,
        Backstabbers_Blade,
        Loonstone_Talisman,
        The_Pipes_Of_Doom,
        The_Clammy_Cowl,
        Leering_Gitshield,

        // Foetid Fetishes
        Spiteshroom_Familiar,
        Moonface_Mommet,
        Staff_Of_Sneaky_Stealin,

        // Venomous Valuables
        Totem_Of_The_Spider_God,
        Headdress_Of_Many_Eyes,
        The_Black_Fang,
        Nibblas_Itty_Ring,
        Earskuttla,
        Web_Strung_Cloak,

        // Glinty Gubbinz That Troggoths Found
        Shiny_Wotnot,
        Glowy_Howzit,
        Pet_Gribbly,

        // Allegiance
        Syari_Screamersquig,
        Aetherquartz_Studded_Hide,
        Shyishan_Spider_Sigils
    };

    class GloomspiteGitzBase : public Unit {
    public:
        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        GloomspiteGitzBase() = delete;

        ~GloomspiteGitzBase() override = default;

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:

        GloomspiteGitzBase(Allegiance allegiance, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
            setAllegiance(allegiance);
        }

        void setAllegiance(Allegiance allegiance);

        void onBeginRound(int battleRound) override;

        void onEndRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRan() override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int woundModifier() const override;

        int moveModifier() const override;

    protected:

        bool inLightOfTheBadMoon() const;

        bool m_movedMoon = false;

        Allegiance m_allegiance = Allegiance::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fangz of the Bad Moon            Yes
// Bad Moon Magic                   Partial/TODO
// Loonatic Inspiration             TODO
// Lunar Squigs                     Yes
// Moonclan Fungus Brew             Yes
// Spiderfang Venom                 Yes
// Troggoth Renawal                 Yes
// Running Riot                     TODO
// Get Some Loonshine Down Em!      TODO
// Monstrous Regeneration           TODO
// Oblivious to Sorcery             TODO
// Deff Grotz of Shyish             TODO
// Drawn to the Afterglow           TODO
// Masters of Feigned Flight        TODO
//

    void Init();

    CommandAbility* CreateImDaBossNoStabEmGood(Unit* source);

} // namespace GloomspiteGitz
