/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace LuminethRealmLords {

    enum class GreatNation : int {
        None,

        Syar,
        Iliatha,
        Zaitrec,
        Ymetrica,
        Alumnia,
        Helon
    };

    enum class CommandTrait : int {
        None,

        // Scinari
        Spellmaster,            // TODO
        Loremaster,             // TODO
        Warmaster,

        // Alarith
        Majestic,
        Enduring,
        //Loremaster,

        // Vanari
        Grand_Strategist,
        Consummate_Warrior,     // TODO
        Astute_Commander,       // TODO

        // Hurakan
        Grand_Windrider,        // TODO
        Swift,
        //Loremaster,

        // Nation specific
        Almighty_Blow,          // Ymetrica TODO
        Goading_Arrogance,      // Syar TODO
        Fast_Learner,           // Zaitrec TODO
        Burning_Gaze,           // Alumnia
        Skyrace_Grand_Champion, // Helon TODO
    };

    enum class Artefact : int {
        None,

        // Scinari
        Phoenix_Stone,
        Silver_Wand,
        Blade_Of_Leaping_Gold,

        // Alarith
        Heartstone_Amulet,
        Ebony_Stone,
        Magmic_Hammer,

        // Vanari
        Syari_Pommel,
        Senlui_Amulet,
        Sun_Stone,

        // Hurakan
        Windblast_Fan,
        Wind_Stone,
        Buffeting_Aspiragillum,

        // Nation specific
        Mountains_Gift, // Ymetrica
        The_Perfect_Blade, // Syar
        Simulacra_Amulet, // Iliatha
        Gift_Of_Celennar, // Zaitrec
        Waystone, // Alumnia
        Metalith_Dust, // Helon
    };

    enum class Lore : int {
        None,

        // Lore of Hysh
        Speed_Of_Hysh,
        Solar_Flare,
        Lambent_Light,
        Etheral_Blessing,
        Total_Eclipse,
        Protection_Of_Hysh,

        // Lore of the Mountains
        Unyielding_Calm,
        Crippling_Vertigo,
        Voice_Of_The_Mountains,
        Living_Fissue,
        Entomb,
        Assault_Of_Stone,

        // Lore of the Winds
        Freezing_Squall,
        Howling_Gale,
        Guiding_Flurries,
        Calming_Zephyr,
        Burning_Simoom,
        Transporting_Vortex,

        // Nation specific
        Overwhelming_Heat, // Zaitrec
    };

    class LuminethBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        LuminethBase() = delete;

        ~LuminethBase() override;

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

    protected:

        LuminethBase(GreatNation nation, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points);
        
        void setNation(GreatNation nation);

        int braveryModifier() const override;

        int woundModifier() const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int castingModifier() const override;

        void onRestore() override;

        int majestic(const Unit *unit);

        void onBeginRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int moveModifier() const override;

        void onStartCombat(PlayerId player) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int unbindingModifier() const override;

    protected:

        GreatNation m_nation = GreatNation::None;

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        mutable int m_aetherQuartzReserve = 1;
        mutable bool m_usedLambentMystics = false;

        lsignal::slot m_majesticConnection;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aetherquartz Reserve             TODO
//   Heightened Reflexes            Yes
//   Heighteden Senses              Yes
//   Magical Boost                  Partial
//   Magical Insight                TODO
// Absorb Despair                   TODO
// Lightning Reactions              TODO
// Shining Company                  TODO
// Deep Thinkers                    TODO
// Move Like the Wind               TODO
// Enduring as Rock                 TODO
// Tectonic Force                   TODO
// Mountain Realm                   TODO
// Redoubled Force                  TODO
// Gleaming Brightness              Yes
// Deplete Reserves                 TODO
// Soul-bond                        Yes
// Unity of Purpose                 TODO
// Strike on Unison                 TODO
// Lambent Mystics                  Partial/TODO
// Claim the Field                  TODO
// Seize the Moment                 TODO
// Gale of Killing Shafts           Yes
// Gone Like the Wind               TODO
//
    void Init();

} // namespace LuminethRealmLords

