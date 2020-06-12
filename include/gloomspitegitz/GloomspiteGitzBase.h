/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef GLOOMSPITEBASE_H
#define GLOOMSPITEBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <gloomspitegitz/BadMoon.h>
#include <array>

namespace GloomspiteGitz {

    //
    // Command Traits
    //
    enum class CommandTrait : int {
        None,

        // BlessingsOfTheBadMoon
        Cunning_Plans,
        Fight_Another_Day,
        Sneaky_Stabba,
        Tough_N_Leathery,
        Dead_Shouty,
        The_Clammy_Hand,

        //GiftsOfTheGloomspite
        Low_Cunning,
        Spiteful_Git,
        Great_Shaman,
        Dodgy_Character,
        Boss_Shaman,
        Loon_Touched,

        // MarksOfTheSpiderGodsFavour
        Monstrous_Mount,
        Master_Spider_Rider,
        Ulutating_Battle_Cry,
        //ToughNLeathery,
        //DeadShouty,
        Creeping_Assault,

        // FortuitousTroggbossTraits
        Tough_As_Rocks,
        Alpha_Trogg,
        Loonskin,
        Pulverising_Grip,
        Mighty_Blow,
        Realmstone_Studded_Hide
    };

    //
    // Artefact
    //
    enum class Artefact : int {
        None,

        // TroglodyticTreasures
        Spiteful_Prodder,
        Backstabbers_Blade,
        Loonstone_Talisman,
        The_Pipes_of_Doom,
        The_Clammy_Cowl,
        Leering_Gitshield,

        // FoetidFetishes
        Spiteshroom_Familiar,
        Moonface_Mommet,
        Staff_of_Sneaky_Stealin,

        // VenomousValuables
        Totem_of_the_Spider_God,
        Headdress_of_Many_Eyes,
        The_Black_Fang,
        Nibblas_Itty_Ring,
        Earskuttla,
        Web_Strung_Cloak,

        // GlintyGubbinzThatTroggothsFound
        Shiny_Wotnot,
        Glowy_Howzit,
        Pet_Gribbly
    };

    class GloomspiteGitzBase : public Unit {
    public:
        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        GloomspiteGitzBase() = default;

        ~GloomspiteGitzBase() override = default;

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:

        GloomspiteGitzBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onBeginRound(int battleRound) override;

        void onEndRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onRan() override;

    protected:

        bool inLightOfTheBadMoon() const;

        bool m_movedMoon = false;

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
//

    void Init();

} // namespace GloomspiteGitz

#endif //GLOOMSPITEBASE_H