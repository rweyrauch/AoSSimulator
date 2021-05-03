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

namespace FleshEaterCourt {

    enum class GrandCourt : int {
        None,
        Morgaunt,
        Hollowmourne,
        Blisterskin,
        Gristlegore,
    };

    enum class Delusion : int {
        None,
        Crusading_Army,
        The_Royal_Hunt,
        The_Feast_Day,
        A_Matter_Of_Honour,
        The_Grand_Tournament,
        Defenders_Of_The_Realm
    };

    enum class CommandTrait : int {
        None,

        // Abhorrant
        Bringer_Of_Death,
        Frenzied_Flesheater,    // TODO
        Savage_Beyond_Reason,
        Majestic_Horror,        // TODO
        Dark_Wizardry,
        Completely_Delusional,  // TODO

        // Courtier
        //Bringer_of_Death,
        //Frenzied_Flesheater,
        //Savage_Beyond_Reason,
        Hulking_Brute,
        Cruel_Taskmaster,       // TODO
        Dark_Acolyte,           // TODO

        // Grand Court specific
        Savage_Chivalry,  // Morgaunt
        Grave_Robber, // Hollowmourne TODO
        Hellish_Orator, // Blisterskin
        Savage_Strike, // Gristlegore
    };

    enum class Artefact : int {
        None,

        // Abhorrant
        Signet_Of_The_First_Court,
        Splintervane_Brooch,
        Blood_River_Chalice,
        The_Grim_Garland,
        The_Dermal_Robe,
        Heart_Of_The_Gargant,

        // Courtier
        Keening_Bone,
        Medal_Of_Madness,
        The_Flayed_Pennant,
        Carrion_Wand,
        The_Fleshform_Raiment,
        The_Bilious_Decanter,

        // Grand Court specific
        Decrepit_Coronet, // Morgaunt
        Corpsefane_Gauntlet, // Hollowmourne
        Eye_Of_Hysh, // Blisterskin
        Ghurish_Mawshard, // Gristlegore
    };

    enum class Lore : int {
        None,

        Bonestorm,
        Spectral_Host,
        Monstrous_Vigour,
        Miasmal_Shroud,
        Deranged_Transformation,
        Blood_Feast,
    };

    enum class MountTrait : int {
        None,

        // Terrorgheist
        Deathly_Fast,
        Razor_Clawed,
        Horribly_Infested,
        Horribly_Resilient,
        Gruesome_Bite,
        Devastating_Scream,

        // Dragon
        //Deathly_Fast,
        //Razor_Clawed,
        Baneful_Breath,
        //Horribly_Resilient,
        Necrotic_Fangs,         // TODO
        Death_From_The_Skies,   // TODO

    };

    class FleshEaterCourts : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        FleshEaterCourts() = delete;

        ~FleshEaterCourts() override = default;

        void setCommandTrait(CommandTrait trait) {
            m_commandTrait = trait;
        }

        void setArtefact(Artefact artefact) {
            m_artefact = artefact;
        }

    protected:
        FleshEaterCourts(GrandCourt court, Delusion delusion, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
            setGrandCourt(court);
            setCourtsOfDelusion(delusion);
        }

        void setGrandCourt(GrandCourt court);

        void setCourtsOfDelusion(Delusion delusion) {
            m_delusion = delusion;
        }

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int moveModifier() const override;

        int runModifier() const override;

        int chargeModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

        int unbindingModifier() const override;

        int woundModifier() const override;

    protected:

        GrandCourt m_grandCourt = GrandCourt::None;
        Delusion m_delusion = Delusion::Crusading_Army;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Feeding Frenzy                   TODO
// Deathless Coutiers               Yes
// Delusion
//    Crusading Army                Yes
//    The Royal Hunt                Yes
//    The Feast Day                 TODO
//    A Matter of Honour            Yes
//    The Grand Tournament          Yes
//    Defenders of the Realm        Yes
// Mourgaunt
//    Blood Loyalty                 Yes
//    Heaving Masses                TODO
//    Savage Chivalry               Yes
// Hollowmourne
//    Shattering Charge             Yes
//    Ravenous Crusaders            TODO
//    Grave Robber                  TODO
// Blisterskin
//    Blistering Speed              Yes
//    Lords of the Burning Skies    TODO
//    Hellish Orator                Yes
// Gristlegore
//    Peerless Ferocity             Yes
//    Call to War                   TODO
//    Savage Strike                 TODO
//

} // namespace FleshEaterCourt
