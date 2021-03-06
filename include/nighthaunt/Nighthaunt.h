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

namespace Nighthaunt {

    enum class Procession : int {
        None,

        Emerald_Host,
        Reikenors_Condemned
    };

    enum class CommandTrait : int {
        None,

        Hatred_Of_The_Living,
        Terrifying_Entity,          // TODO
        Lingering_Spirit,
        Spiteful_Spirit,            // TODO
        Cloaked_In_Shadow,
        Ruler_Of_The_Spirit_Hosts,  // TODO

        // Processions
        Lord_Of_The_Host,  // Emerald Host TODO
    };

    enum class Artefact : int {
        None,

        // Weapons
        Shadows_Edge,
        Reaper_Of_Sorrows,
        Balefire_Blade,
        Slitter,
        Headsmans_Judgement,
        Shrieking_Blade,

        // Relics
        Cloak_Of_The_Waxing_Moon,
        Pendant_Of_The_Fell_Wind,
        Dreadbolt_Ring,
        Mirror_Of_Screaming_Souls,
        Midnight_Tome,
        Covetous_Familiar,

        // Lanterns (Guardian of Souls)
        Lightshard_Of_The_Harvest_Moon,
        Wychlight_Lantern,
        Beacon_Of_Nagashizzar,

        // Processions
        The_Traitor_Knights_Blade,  // Emerald Host
        Corpse_Candle,  // Reikenor's Condemned
    };

    enum class Lore : int {
        None,

        Soul_Cage,
        Spirit_Drain,
        Lifestealer,
        Reaping_Scythe,
        Shademist,
        Spectral_Tether,

    };


    class Nighthaunt : public Unit {
    public:
        Nighthaunt() = delete;

        ~Nighthaunt() override;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setProcession(Procession procession);

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

    protected:

        Nighthaunt(Procession procession, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points);

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int auraOfDread(const Unit *unit);

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int woundModifier() const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    protected:
        Procession m_procession = Procession::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        lsignal::slot m_auraOfDreadSlot;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Dread                    Yes
// Deathless Spirits                Yes
// From the Underworlds They Come   TODO
// Feed on Terror                   TODO
// Wave of Terror                   TODO
// Spectral Summons                 TODO
// The Emerald Host
//    The Emerald Curse             TODO
//    Knights of Regret             TODO
// Reikenor's Condemned
//    Unrelenting Taskmasters       TODO
//    Acolyte of the Grimhaller     TODO
//    Death Comes Swiftly           TODO
//

    void Init();

} // namespace Nighthaunt
