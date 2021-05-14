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

    enum class Legion : int {
        None,

        Grand_Host_Of_Nagash,
        Legion_Of_Sacrament,
        Legion_Of_Blood,
        Legion_Of_Night
    };

    enum class CommandTrait : int {
        None,

        // Grand Host
        Master_Of_Death,            // TODO
        Chosen_Champion,
        Bane_Of_The_Living,
        Aura_Of_Ages,
        Ancient_Strategist,
        Lord_Of_Nagashizzar,

        // Legion of Sacrament
        Emissary_Of_The_Master,
        Mark_Of_The_Favoured,       // TODO
        Dark_Acolyte,
        Mastery_Of_Death,           // TODO
        Peerless_Commander,         // TODO
        Bound_To_The_Master,        // TODO

        // Legion of Blood
        Swift_Strikes,
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

        // Death
        Ruler_Of_The_Night,         // TODO
        Predator_Of_The_Shadows,
        Death_Incarnate,            // TODO
        Master_Of_The_Black_Arts,
        Red_Fury,                   // TODO
        Supernatural_Horror,        // TODO
    };

    enum class Artefact : int {
        None,

        // Grand Host
        Deathforged_Chain,
        Balefire_Lantern,
        Grave_Sand_Timeglass,
        Ossific_Diadem,
        Amethyst_Shard,
        Terrorgheist_Mantle,

        // Legion of Sacrament
        Spiritcage,
        Shroud_Of_Darkness,
        Asylumaticae,
        Wristbands_Of_Black_Gold,
        Azyrbane_Standard,
        Black_Gem,

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

    };

    enum class Lore : int {
        None,

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
        Vile_Transference,      // TODO
        Amethystine_Pinions,
        Soulpike,               // TODO
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

        SoulblightBase(Legion legion, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
            setLegion(legion);
        }

        void setLegion(Legion legion);

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

        Legion m_legion = Legion::Grand_Host_Of_Nagash;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Unquiet Dead                 TODO
// Deathless Minons                 TODO
// Endless Legions                  TODO
// Grand Host of Nagash
//    Chosen Guardians              TODO
//    Legions Innumerable           TODO
// Legion of Sacrament
//    The Master's Teachings        TODO
//    The Black Disciples           TODO
// Legion of Blood
//    Immortal Majesty              TODO
//    Favoured Retainers            TODO
// Legion of Night
//    The Bait                      TODO
//    Ageless Cunning               TODO
// Soulblight
//    Deathless Thralls             TODO
//    The Bloodlines                TODO
//      Dragon Warriors             TODO
//      Lords of Night              TODO
//      Necromantic                 TODO
//      Swift Death                 TODO
//

    void Init();

} // namespace Soulblight
