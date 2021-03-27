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
#include <array>

namespace DaughtersOfKhaine {

    enum class Temple : int {
        None = 0,
        Hagg_Nar,
        Draichi_Ganeth,
        The_Kraith,
        Khailebron,
        Zainthar_Kai,
        Khelt_Nar
    };

    enum class CommandTrait : int {
        None,

        // Paragons of Murder
        Bathed_In_Blood,        // TODO
        Zealous_Orator,         // TODO
        Sacrificer_Overseer,    // TODO
        Terrifying_Beauty,      // TODO
        Master_Of_Poisons,      // TODO
        True_Believer,          // TODO

        // Masters of Blood Magic
        Arcane_Mastery,         // TODO
        Writhing_Coils,         // TODO
        Fearsome_Presence,      // TODO

        // Morathi's Right Hand
        Veteran_Of_The_Cathirar_Dhule,  // TODO
        Impenentrable_Scales,   // TODO
        Fuelled_By_Revenge,     // TODO

        // Temple specific
        Devoted_Disciple,       // Hagg Nar TODO
        Mistress_Of_Illusion,   // Khailebron TODO
        Curse_Of_The_Bloody_Handed, // Zainthar Kai TODO
        Bathe_In_Their_Blood,   // The Kraith TODO
        The_Circling_Flock,     // Khelt Nar TODO
        Victor_Of_Yaithril,     // Draichi Ganeth TODO
    };

    enum class Artefact : int {
        None,

        // Hero
        Crown_Of_Woe,
        Shadracars_Fangs,
        Amulet_Of_Dark_Fire,
        Crone_Blade,
        Thousand_And_One_Dark_Blessings,
        Bloodbane_Venom,

        // Bloodwrack Medusa
        Shadow_Stone,
        Rune_Of_Ulgu,
        The_Mirror_Glaive,
        Sevenfold_Shadow,
        Crystal_Heart,
        Shade_Claw,

        // Priest
        Blood_Sigil,
        Iron_Circlet,
        Rune_Of_Khaine,
        Crimson_Shard,
        Khainite_Pendant,
        Hagbrew,

        // Temple specific
        The_Ulfuri,  // Hagg Nar
        Whisperdeath, // Khailebron
        The_Darksword,  // Draichi Ganeth
        Venom_Of_Nagendra,  // The Kraith
        Crimson_Talisman, // Zainthar Kai
        Gaisas_Falx, // Khelt Nar
    };

    enum class Lore : int {
        None,

        Steed_Of_Shadows,
        Pit_Of_Shades,
        Mirror_Dance,
        The_Withering,
        Mindrazor,
        Shroud_Of_Despair,
    };

    enum class Prayer : int {
        None,

        Catechism_Of_Murder,
        Blessing_Of_Khaine,
        Martyrs_Sacrifice,
        Crimson_Rejuvination,
        Covenant_Of_The_Iron_Heart,
        Sacrament_Of_Blood,
    };

    class DaughterOfKhaine : public Unit {
    public:
        DaughterOfKhaine() = default;

        ~DaughterOfKhaine() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setTemple(Temple temple);

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

        int getBloodRiteRound() const { return m_battleRound + m_bloodRiteModifier; }

        void configureCommon();

    protected:
        DaughterOfKhaine(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        Rerolls runRerolls() const override;

        Rerolls chargeRerolls() const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int braveryModifier() const override;

        int weaponRend(const Weapon *weapon, const Unit *target,
                       int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

        void onEndMovement(PlayerId player) override;

        void onRestore() override;

    protected:

        Temple m_temple = Temple::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
        int m_bloodRiteModifier = 0;

        bool m_usedCirclingFlock = false;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fanatical Faith                  Yes
// Blood Rites                      Yes
//    Quickening Bloodlust          Yes
//    Headlong Fury                 Yes
//    Zealot's Rage                 Yes
//    Slaughter's Strength          Yes
//    Unquenchable Fervour          Yes
// Hagg Nar
//   Daughters of the First Temple  Yes
//   Devoted Disciple               Yes
//   Send Forth the Cauldrons       TODO
//   The Ulfuri                     TODO
// Draichi Ganeth
//   Bladed Killers                 Yes
//   Victor Of Yaithril             Yes
//   Death's Kiss                   TODO
//   A Thousand Bladeforms          Yes
// The Kraith
//   Disciples of Slaughter         Yes
//   Venom of Nagendra              TODO
//   Bathe in Their Blood           TODO
//   Inspired by Carnage            Yes
// Khailebron
//   Concealment and Stealth        Yes
//   Mistress of Illusion           Yes
//   Whisperdeath                   TODO
//   Masters of the Shadowpaths     TODO
// Khelt Nar
//   Strike and Fade                Yes
//   The Circling Flock             Yes
//   Gaisas FalX                    TODO
//   Bleed the Mind                 TODO
// Zainthar Kai
//   Khaine's Essence               Yes
//   Vault of the First Brood       TODO
//   Curse of the Bloody-Handed     Yes
//   Power in the Blood             Yes
//   Crimson Talisman               TODO
//

    void Init();

} // namespace DaughtersOfKhaine
