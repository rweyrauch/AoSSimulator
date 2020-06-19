/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LEGIONOFNAGASH_H
#define LEGIONOFNAGASH_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Death {

    enum class Legion : int {
        Grand_Host_of_Nagash,
        Legion_of_Sacrament,
        Legion_of_Blood,
        Legion_of_Night
    };

    enum class CommandTrait : int {
        None,

        // Grand Host
        Master_of_Death,
        Chosen_Champion,
        Bane_of_the_Living,
        Aura_of_Ages,
        Ancient_Strategist,
        Lord_of_Nagashizzar,

        // Legion of Sacrament
        Emissary_of_the_Master,
        Mark_of_the_Favoured,
        Dark_Acolyte,
        Mastery_of_Death,
        Peerless_Commander,
        Bound_to_the_Master,

        // Legion of Blood
        Swift_Strikes,
        Soul_Crushing_Contempt,
        Aristocracy_of_Blood,
        Aura_of_Dark_Majesty,
        Walking_Death,
        Sanguine_Blur,

        // Legion of Night
        Above_Suspicion,
        Swift_Form,
        Unbending_Will,
        Merciless_Hunter,
        Unholy_Impetus,
        Terrifying_Visage,

        // Soulblight
        Curse_of_the_Revenant,
        Deathless_Duellist,
        Transfix,
        Mist_Form,
        Killing_Blow,
        Blood_Fury,

        // Death
        Ruler_of_the_Night,
        Predator_of_the_Shadows,
        Death_Incarnate,
        Master_of_the_Black_Arts,
        Red_Fury,
        Supernatural_Horror,
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
        Shroud_of_Darkness,
        Asylumaticae,
        Wristbands_of_Black_Gold,
        Azyrbane_Standard,
        Black_Gem,

        // Legion of Blood
        Ring_of_Domination,
        Shadeglass_Decanter,
        Orb_of_Enchantment,
        Soulbound_Garments,
        Oubliette_Arcana,
        Amulet_of_Screams,

        // Legion of Night
        Vial_of_the_Pure_Blood,
        Shard_of_Night,
        Gem_of_Exsanguination,
        Chiropteric_Cloak,
        Morbhegs_Claw,
        Curseblade,

    };

    enum class Lore : int {
        None,

        // Deathmages
        Overwhelming_Dread,
        Fading_Vigour,
        Spectral_Grasp,
        Prison_of_Grief,
        Decrepify,
        Soul_Harvest,

        // Vampire
        Blades_of_Shyish,
        Spirit_Gale,
        Vile_Transference,
        Amethystine_Pinions,
        Soulpike,
        Amaranthine_Orb,

    };

    class LegionOfNagashBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        LegionOfNagashBase() = default;

        ~LegionOfNagashBase() override = default;

        void setLegion(Legion legion);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:

        LegionOfNagashBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void deathlyInvocations(int numUnits, double range);

    protected:

        Legion m_legion = Legion::Grand_Host_of_Nagash;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Unquiet Dead                 TODO
// Deathless Thralls                TODO
// The Bloodlines                   TODO
//    Dragon Warriors               TODO
//    Lords of Night                TODO
//    Necromantic                   TODO
//    Swift Death                   TODO
// Deathless Minons                 TODO
//

    void Init();

} // namespace Death

#endif //LEGIONOFNAGASH_H