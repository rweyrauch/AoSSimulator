/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SERAPHON_H
#define SERAPHON_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Seraphon {

    enum class WayOfTheSeraphon : int {
        Starborne,
        Coalesced
    };

    enum class Constellation : int {
        None,
        Koatls_Claw,        // Coalesced
        Thunder_Lizard,     // Coalesced
        Dracothions_Tail,   // Starborne
        Fangs_of_Sotek,      // Starborne
    };

    //
    // Command Traits
    //
    enum class CommandTrait : int {
        None,

        // Slann
        Arcane_Might,
        Vast_Intellect,
        Great_Rememberer,

        // Saurus
        Disciplined_Fury,
        Thickly_Scaled_Hide,
        Mighty_Warleader,

        // Skink
        Master_of_Star_Rituals,
        Nimble,
        Cunning
    };

    //
    // Artefacts
    //
    enum class Artefact : int {
        None,

        // TreasuresOfTheOldOnes
        Zoetic_Dial,
        Light_of_Dracothian,
        Prism_of_Amyntok,
        Itxi_Grubs,
        Plaque_of_Dominion,
        Throne_of_the_Lost_Gods,

        // CelestialRelicsOfTheWarrior
        Blade_of_Realities,
        Sigils_of_the_Prime_Hunter,
        Blood_Rage_Pendant,

        // VestmentsOfThePriesthood
        Incandescent_Rectrices,
        Cloak_of_Feathers,
        Sacred_Stegadon_Helm
    };

    //
    // Spells
    //
    enum class Lore : int {
        None,

        // LoreOfCelestialDomination
        Celestial_Apotheosis,
        Walk_Between_Realms,
        Mystical_Unforging,
        Celestial_Equilibrium,
        Stellar_Tempest,
        Drain_Magic,

        // LoreOfCelestialManipulation
        Celestial_Harmony,
        Hand_of_Glory,
        Extend_Astromatrix,
        Fiery_Convocation,
        Bind_Endless_Spell,
        Tide_of_Serpents
    };

    class SeraphonBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SeraphonBase() = default;

        ~SeraphonBase() override = default;

        void setWayOfTheSeraphon(WayOfTheSeraphon way, Constellation constellation);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:

        SeraphonBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        int moveModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int braveryModifier() const override;

        Wounds targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll,
                                          int woundRoll) const override;

        int woundModifier() const override;

    protected:

        WayOfTheSeraphon m_way = WayOfTheSeraphon::Coalesced;
        Constellation m_constellation = Constellation::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Contemplations of the Ancient Ones   TODO
// Sacred Asterisms                 TODO
//   The Great Drake                TODO
//   The Hunter's Steed             TODO
//   The Sage's Staff               TODO
// Cold Blooded                     TODO
// Predatory Fighters               Yes
// Primeval Domain                  TODO
// Scaly Skin                       Yes
// Unfeeling                        Yes
// Celestial Conjuration            TODO
// Lords of Space and Time          TODO
// Appear on Command                TODO
// First to Battle                  Yes
// Savagery Incarnate               Yes
// Mighty Beasts of War             Yes
//

    void Init();

} // namespace Seraphon

#endif //SERAPHON_H