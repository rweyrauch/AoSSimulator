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
        Ravagers,
        Cabalists,
        Despoilers,
        Host_Of_The_Everchosen,
        Idolators
    };

    enum class MarkOfChaos : int {
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
        Unquestioned_Resolve,
        Favoured_Of_The_Pantheon,
        Eternal_Vendetta,
        Flames_Of_Spite,
        Master_Of_Deception,

        // Cabalists
        //Bolstered_by_Hate,
        Lord_Of_Terror,
        //Favoured_of_the_Pantheon,
        Mighty_Ritualist,
        Blasphemous_Influence,
        All_For_One,

        // Despoilers
        //Bolstered_by_Hate,
        //Lord_of_Terror,
        Lightning_Reflexes,
        Radiance_Of_Dark_Glory,
        Distorting_Miasma,
        Paragon_Of_Ruin,

        // Idolators
        //Bolstered_by_Hate,
        //Lord_of_Terror,
        //Favoured_of_the_Pantheon,
        Fiery_Orator,
        Bane_Of_False_Idols,
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
        Blessings_Of_Chaos_Undivided,
        Blessings_Of_Nurgle,
        Blessings_Of_Khorne,
        Blessings_Of_Slaanesh,
        Blessings_Of_Tzeentch
    };

    class SlavesToDarknessBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SlavesToDarknessBase() = default;

        ~SlavesToDarknessBase() override = default;

        void setDamnedLegion(DamnedLegion legion);

        void setMarkOfChaos(MarkOfChaos mark);

    protected:

        SlavesToDarknessBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        void onFriendlyUnitSlain() override;
        void onEnemyUnitSlain(const Unit* unit) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls runRerolls() const override;
        Rerolls chargeRerolls() const override;

        void onEndCombat(PlayerId player) override;

        void onRestore() override;

    private:

        bool m_haveDarkApotheosis = false;
        bool m_haveDaemonicLegions  = false;
        bool m_haveUnholyResilience = false;
        bool m_haveFlamesOfChaos = false;
        bool m_haveIronFlesh = false;
        bool m_haveMurderousMutation  = false;
        bool m_haveSlaughtersStrength  = false;
        bool m_haveSpawndom  = false;

        DamnedLegion m_legion = DamnedLegion::Ravagers;
        MarkOfChaos m_markOfChaos = MarkOfChaos::Undivided;
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
//    Dark Apotheosis               TODO
//    Daemonic Legions              Yes
//    Unholy Resilience             Yes
//    Snubbed by the Gods           Yes
//    Flames of Chaos               Yes
//    Iron Flesh                    Yes
//    Murderous Mutation            TODO
//    Slaughter's Strength          TODO
//    Spawndom                      Yes
// Ravagers
//    Glory for the Taking          TODO
//    Rally the Tribes              TODO
// Cabalists
//    Ritual of Sorcerous Might     TODO
//    Ritual of Corruption          TODO
// Despoilers
//    Sacrilegious Might            TODO
//    Blessed by the Unholy         TODO
//    Twisted Dominion              TODO
// Host of the Everchosen
//    Fearless in His Presence      TODO
//    The Will of the Everchosen    TODO
//    Dark Prophecy                 TODO
// Idolators
//    Panoply of Ruin               TODO
//    Blessed of Chaos              TODO
//    Idolator Lords                TODO
//    Destroy the False Idols       TODO
//    Desecrate                     TODO
//

} // SlavesToDarkness
