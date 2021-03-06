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
#include <array>

namespace SlavesToDarkness {

    enum class DamnedLegion : int {
        None,

        Ravagers,
        Cabalists,
        Despoilers,
        Host_Of_The_Everchosen,
        Idolators
    };

    enum class MarkOfChaos : int {
        None,

        Undivided,
        Nurgle,
        Khorne,
        Slaanesh,
        Tzeentch
    };

    enum class Lore : int {
        None,

        // Lore of the Damned
        Binding_Damnation,
        Spite_Tongue_Curse,
        Whispers_Of_Chaos,
        Mask_Of_Darkness,
        Call_To_Glory,
        Ruinous_Vigour
    };

    enum class CommandTrait : int {
        None,

        // Ravagers
        Bolstered_By_Hate,
        Unquestioned_Resolve,           // TODO
        Favoured_Of_The_Pantheon,       // TODO
        Eternal_Vendetta,
        Flames_Of_Spite,
        Master_Of_Deception,

        // Cabalists
        //Bolstered_by_Hate,
        Lord_Of_Terror,
        //Favoured_of_the_Pantheon,
        Mighty_Ritualist,               // TODO
        Blasphemous_Influence,          // TODO
        All_For_One,                    // TODO

        // Despoilers
        //Bolstered_by_Hate,
        //Lord_of_Terror,
        Lightning_Reflexes,
        Radiance_Of_Dark_Glory,
        Distorting_Miasma,              // TODO
        Paragon_Of_Ruin,                // TODO

        // Idolators
        //Bolstered_by_Hate,
        //Lord_of_Terror,
        //Favoured_of_the_Pantheon,
        Fiery_Orator,                   // TODO
        Bane_Of_False_Idols,            // TODO
        Smite_The_Unbeliever,
    };

    enum class Artefact : int {
        None,

        // Ravagers
        Hellfire_Sword,
        Blasphemous_Cuirass,
        Helm_Of_The_Oppressor,
        Cloak_Of_The_Relentless_Conqueror,
        Mark_Of_The_High_Favoured,
        Desecrator_Gauntlets,

        // Cabalists
        Soul_Feeder,
        Black_Athame,
        Infernal_Puppet,
        Spelleater_Pendant,
        Scroll_Of_Dark_Unravelling,
        Spell_Familiar,

        // Despoilers
        Crown_Of_Hellish_Adoration,
        Helm_Of_Many_Eyes,
        Armour_Of_Tortured_Souls,
        Diabolic_Mantle,
        Doombringer_Blade,
        Realmwarpers_Twist_Rune
    };

    enum class Prayer : int {
        None,

        // Idolators
        Blessings_Of_Chaos_Undivided,   // TODO
        Blessings_Of_Nurgle,            // TODO
        Blessings_Of_Khorne,            // TODO
        Blessings_Of_Slaanesh,          // TODO
        Blessings_Of_Tzeentch           // TODO
    };

    class SlavesToDarknessBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SlavesToDarknessBase() = delete;

        ~SlavesToDarknessBase() override;

        void setMarkOfChaos(MarkOfChaos mark);

        bool setCommandTrait(CommandTrait trait);

        bool setArtefact(Artefact artefact);

    protected:

        SlavesToDarknessBase(DamnedLegion legion, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points);

        void setDamnedLegion(DamnedLegion legion);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        void onEnemyUnitSlain(const Unit* unit) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls runRerolls() const override;

        Rerolls chargeRerolls() const override;

        void onEndCombat(PlayerId player) override;

        void onRestore() override;

        int woundModifier() const override;

        int rollChargeDistance() override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int lordOfTerror(const Unit* unit);

        void onEndHero(PlayerId player) override;

        int prayerModifier() const override;

        bool battleshockRequired() const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        bool m_haveDarkApotheosis = false;
        bool m_haveDaemonicLegions  = false;
        bool m_haveUnholyResilience = false;
        bool m_haveFlamesOfChaos = false;
        bool m_haveIronFlesh = false;
        bool m_haveMurderousMutation  = false;
        bool m_haveSlaughtersStrength  = false;
        bool m_haveSpawndom  = false;

        lsignal::slot m_lordOfTerrorSlot;

        DamnedLegion m_legion = DamnedLegion::Ravagers;
        MarkOfChaos m_markOfChaos = MarkOfChaos::Undivided;

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Chaos
//    Aura of Khorne                Yes
//    Aura of Tzeentch              Yes
//    Aura of Nurgle                Yes
//    Aura of Slaanesh              Yes
//    Aura of Chaos Undivided       Yes
// Eye of the Gods
//    Dark Apotheosis               Yes
//    Daemonic Legions              Yes
//    Unholy Resilience             Yes
//    Snubbed by the Gods           Yes
//    Flames of Chaos               Yes
//    Iron Flesh                    Yes
//    Murderous Mutation            Yes
//    Slaughter's Strength          Yes
//    Spawndom                      Yes
// Ravagers
//    Glory for the Taking          TODO
//    Rally the Tribes              TODO
// Cabalists
//    Ritual of Sorcerous Might     TODO
//    Ritual of Corruption          TODO
// Despoilers
//    Sacrilegious Might            Partial/TODO
//    Blessed by the Unholy         Yes
//    Twisted Dominion              TODO
// Host of the Everchosen
//    Fearless in His Presence      Yes
//    The Will of the Everchosen    TODO
//    Dark Prophecy                 TODO
// Idolators
//    Panoply of Ruin               Yes
//    Blessed of Chaos              Yes
//    Idolator Lords                Yes
//    Destroy the False Idols       Yes
//    Desecrate                     TODO
//

} // SlavesToDarkness
