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

namespace IdonethDeepkin {

    enum class Enclave : int {
        Custom = 0,
        Ionrach,
        Dhom_Hain,
        Fuethan,
        Morphann,
        Nautilar,
        Briomdar
    };

    enum class CommandTrait : int {
        None,

        Merciless_Raider,       // TODO
        Hunter_Of_Souls,        // TODO
        Unstoppable_Fury,       // TODO
        Born_From_Agony,        // TODO
        Nightmare_Legacy,       // TODO
        Lord_Of_Storm_And_Sea,  // TODO

        // Enclave
        Emissary_Of_The_Deep_Places, // Ionrach TODO
    };

    enum class Artefact : int {
        None,

        // Akhelian
        Sanguine_Pearl,
        Potion_Of_Hateful_Frenzy,
        Ankusha_Spear,
        Armour_Of_The_Cythai,
        Bioshock_Shell,
        Abyssal_Blade,

        // Idoneth
        Rune_Of_The_Surging_Tide,
        Black_Pearl,
        Lliandras_Last_Lament,
        Terrornight_Venom,
        Cloud_Of_Midnight,
        Whorlshell,

        // Isharann
        Steelshell_Pearl,
        Mind_Flare,
        Dritchleech,
        Auric_Lantern,
        Disharmony_Stones,
        Brain_Barnacles,

        // Arcane
        Arcane_Pearl,
        Sands_Of_Infinity,
        Coral_Ring,
        Bauble_Of_Buoyancy,
        Kraken_Tooth,
        Augury_Shells,
    };

    enum class Lore : int {
        None,

        Steed_Of_Tides,
        Abyssal_Darkness,
        Vorpal_Maelstrom,
        Pressure_Of_The_Deep,
        Tide_Of_Fear,
        Arcane_Corrasion,

        // Enclave specific
        Freezing_Mists,  // Morphann
        Protective_Barrier, // Nautilar

    };

    enum class MountTrait : int {
        None,

        // Deepmare
        Swift_Finned_Impaler,
        Savage_Ferocity,
        Voidchill_Darkness,

        // Leviadon
        Ancient,
        Denizen_Of_The_Darkest_Depths,
        Reverberating_Carapace,

    };
    class IdonethDeepkinBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        IdonethDeepkinBase() = delete;

        ~IdonethDeepkinBase() override = default;

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:

        IdonethDeepkinBase(Enclave enclave, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
            setEnclave(enclave);
        }

        void setEnclave(Enclave enclave);

    protected:

        Enclave m_enclave = Enclave::Custom;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Tide of Death
//   Low Tide                       TODO
//   Flood Tide                     TODO
//   High Tide                      TODO
//   Ebb Tide                       TODO
// Isharann Rituals
//   Ritual of Erosion              TODO
//   Ritual of Rousing              TODO
//   Ritual of the Tempest          TODO
// Ionrach
//   Strong in Magic                TODO
//   Emissary of the Deep Places    TODO
// Dhom-hain
//   Savage Fighters                TODO
//   Deep Questors                  TODO
// Fuethan
//   Revel in Slaughter             TODO
//   Fiercest of Creatures          TODO
// Morphann
//   Soul-mage Adepts               TODO
//   Freezing Mists                 TODO
// Nautilar
//   Consumate Defenders            TODO
//   Protective Barrier             TODO
// Briomdar
//   Supreme Soulscryers            TODO
//   Unstoppable Raiders            TODO
//

    void Init();

} // namespace IdonethDeepkin

