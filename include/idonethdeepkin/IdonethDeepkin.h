/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IDONETHDEEPKINBASE_H
#define IDONETHDEEPKINBASE_H

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

        Merciless_Raider,
        Hunter_of_Souls,
        Unstoppable_Fury,
        Born_From_Agony,
        Nightmare_Legacy,
        Lord_of_Storm_and_Sea,

        // Enclave
        Emissary_of_the_Deep_Places, // Ionrach

    };

    enum class Artefact : int {
        None,

        // Akhelian
        Sanguine_Pearl,
        Potion_of_Hateful_Frenzy,
        Ankusha_Spear,
        Armour_of_the_Cythai,
        Bioshock_Shell,
        Abyssal_Blade,

        // Idoneth
        Rune_of_the_Surging_Tide,
        Black_Pearl,
        Lliandras_Last_Lament,
        Terrornight_Venom,
        Cloud_of_Midnight,
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
        Sands_of_Infinity,
        Coral_Ring,
        Bauble_of_Buoyancy,
        Kraken_Tooth,
        Augury_Shells,
    };

    enum class Lore : int {
        None,

        Steed_of_Tides,
        Abyssal_Darkness,
        Vorpal_Maelstrom,
        Pressure_of_the_Deep,
        Tide_of_Fear,
        Arcane_Corrasion,

        // Enclave specific
        Freezing_Mists,  // Morphann
        Protective_Barrier, // Nautilar

    };

    class IdonethDeepkinBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        IdonethDeepkinBase() = default;

        ~IdonethDeepkinBase() override = default;

        void setEnclave(Enclave enclave);

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);
    protected:

        IdonethDeepkinBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

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

#endif // IDONETHDEEPKINBASE_H
