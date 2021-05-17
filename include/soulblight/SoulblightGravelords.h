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

namespace Soulblight {

    enum class CursedBloodline : int {
        None,

        Legion_Of_Blood,
        Legion_Of_Night,
        Vyrkos_Dynasty,
        Kastelai_Dynasty,
        Avengorii_Dynasty
    };

    enum class CommandTrait : int {
        None,

        // Legion of Blood
        Premeditated_Violence,      // TODO Swift_Strikes
        Soul_Crushing_Contempt,
        Aristocracy_Of_Blood,
        Aura_Of_Dark_Majesty,
        Walking_Death,              // TODO
        Sanguine_Blur,

        // Legion of Night
        Above_Suspicion,            // TODO
        Swift_Form,
        Unbending_Will,
        Merciless_Hunter,
        Unholy_Impetus,
        Terrifying_Visage,

        // Soulblight
        Curse_Of_The_Revenant,
        Deathless_Duellist,
        Transfix,                   // TODO
        Mist_Form,                  // TODO
        Killing_Blow,
        Blood_Fury,

        // Vyrkos Dynasty
        Pack_Alpha,                 // TODO
        Driven_By_Deathstench,      // TODO
        Kin_Of_The_Wolf,            // TODO
        Hunters_Snare,              // TODO
        Spoor_Trackers,             // TODO
        United_By_Blood,            // TODO

        // Kastelai Dynasty
        Beacon_Of_Bloodshed,        // TODO
        Master_Of_Retaliation,      // TODO
        Power_In_The_Blood,         // TODO
        Rousing_Commander,          // TODO
        Swift_And_Deadly,           // TODO
        A_Craving_For_Massacre,     // TODO

        // Avengorii Dynasty
        An_Eye_For_An_Eye,          // TODO
        Torment_Driven_Throes,      // TODO
        Unhinged_Rampager,          // TODO
    };

    enum class Artefact : int {
        None,

        // Legion of Blood
        Ring_Of_Domination,
        Shadeglass_Decanter,
        Orb_Of_Enchantment,
        Soulbound_Garments,
        Oubliette_Arcana,
        Amulet_Of_Screams,

        // Legion of Night
        Vial_Of_The_Pure_Blood,
        Shard_Of_Night,
        Gem_Of_Exsanguination,
        Chiropteric_Cloak,
        Morbhegs_Claw,
        Curseblade,

        // Kastelai Dynasty
        Sword_Of_The_Red_Seneschals,
        Bloodsaints_Shield,
        Stardard_Of_The_Crimson_Keep,
        Grave_Sand_Shard,
        Fragment_Of_The_Keep,
        The_Red_Casket,

        // Vyrkos Dynasty
        Ulfenkarnian_Phylactery,
        Cloak_Of_The_Night_Prowler,
        Sangsyron_SP,
        Vilnas_Fang,
        Terminus_Cloak,
        Standard_Of_The_Ulfenwatch,

        // Avengorii Dynasty
        Breath_Of_The_Void_Maw,
        Ghorvars_Collar,
        The_Furious_Crown,
    };

    enum class Mutation : int {
        Maddening_Hunger,
        Urges_Of_Atrocity,
        Nullblood_Construct,
    };

    enum class Lore : int {
        None,

        Invigorating_Aura,      // TODO

        // Deathmages
        Overwhelming_Dread,
        Fading_Vigour,          // TODO
        Spectral_Grasp,         // TODO
        Prison_Of_Grief,        // TODO
        Decrepify,
        Soul_Harvest,           // TODO

        // Vampire
        Blades_Of_Shyish,
        Spirit_Gale,
        Soulpike,               // TODO
        Amethystine_Pinions,
        Vile_Transference,      // TODO
        Amaranthine_Orb,
    };

    class SoulblightBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SoulblightBase() = delete;

        ~SoulblightBase() override;

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

    protected:

        SoulblightBase(CursedBloodline bloodline, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
            setBloodline(bloodline);
        }

        void setBloodline(CursedBloodline bloodline);

        void deathlyInvocations(int numUnits, double range);

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls chargeRerolls() const override;

        void onStartCombat(PlayerId player) override;

        int runModifier() const override;

        int woundModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int castingModifier() const override;

        int unbindingModifier() const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int moveModifier() const override;

        Rerolls battleshockRerolls() const override;

        void onEnemyModelSlain(int numSlain, Unit *enemyUnit, Wounds::Source source) override;

        int terrifyingVisage(const Unit *unit);

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

    protected:

        lsignal::slot m_terrifyVisageSlot;

        CursedBloodline m_bloodline = CursedBloodline::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Unquiet Dead                 TODO
// Locus of Shyish                  TODO
// Deathless Minions                TODO
// Endless Legions                  TODO
// Reanimated Horrors               TODO
// Deathly Invocations              TODO
// Legion of Blood
//    Immortal Majesty              TODO
//    Favoured Retainers            TODO
// Legion of Night
//    The Bait                      TODO
//    Ageless Cunning               TODO
// Vyrkos Dynasty
//    Strength of the Pack...       TODO
//    Strength of the Wolf...       TODO
//

    void Init();

} // namespace Soulblight
