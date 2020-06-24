/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLESHEATERCOURTS_H
#define FLESHEATERCOURTS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace FleshEaterCourt {

    enum class GrandCourt : int {
        None = 0,
        Morgaunt,
        Hollowmourne,
        Blisterskin,
        Gristlegore,
    };

    enum class Delusion : int {
        Crusading_Army = 0,
        The_Royal_Hunt,
        The_Feast_Day,
        A_Matter_of_Honour,
        The_Grand_Tournament,
        Defenders_of_the_Realm
    };

    enum class CommandTrait : int {
        None,

        // Abhorrant
        Bringer_of_Death,
        Frenzied_Flesheater,
        Savage_Beyond_Reason,
        Majestic_Horror,
        Dark_Wizardry,
        Completely_Delusional,

        // Courtier
        //Bringer_of_Death,
        //Frenzied_Flesheater,
        //Savage_Beyond_Reason,
        Hulking_Brute,
        Cruel_Taskmaster,
        Dark_Acolyte,

        // Grand Court specific
        Savage_Chivalry,  // Morgaunt
        Grave_Robber, // Hollowmourne
        Hellish_Orator, // Blisterskin
        Savage_Strike, // Gristlegore
    };

    enum class Artefact : int {
        None,

        // Abhorrant
        Signet_of_the_First_Court,
        Splintervane_Brooch,
        Blood_River_Chalice,
        The_Grim_Garland,
        The_Dermal_Robe,
        Heart_of_the_Gargant,

        // Courtier
        Keening_Bone,
        Medal_of_Madness,
        The_Flayed_Pennant,
        Carrion_Wand,
        The_Fleshform_Raiment,
        The_Bilious_Decanter,

        // Grand Court specific
        Decrepit_Coronet, // Morgaunt
        Corpsefane_Gauntlet, // Hollowmourne
        Eye_of_Hysh, // Blisterskin
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
        Necrotic_Fangs,
        Death_From_the_Skies,

    };

    class FleshEaterCourts : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        FleshEaterCourts() = default;

        ~FleshEaterCourts() override = default;

        void setGrandCourt(GrandCourt court);

        void setCourtsOfDelusion(Delusion delusion) {
            m_delusion = delusion;
        }

        void setCommandTrait(CommandTrait trait) {
            m_commandTrait = trait;
        }

        void setArtefact(Artefact artefact) {
            m_artefact = artefact;
        }

    protected:
        FleshEaterCourts(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int moveModifier() const override;

        int runModifier() const override;

        int chargeModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds) override;

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
// Deathless Coutiers               TODO
// Delusion
//    Crusading Army                TODO
//    The Royal Hunt                TODO
//    The Feast Day                 TODO
//    A Matter of Honour            TODO
//    The Grand Tournament          TODO
//    Defenders of the Realm        TODO
// Mourgaunt
//    Blood Loyalty                 TODO
//    Heaving Masses                TODO
//    Savage Chivalry               TODO
// Hollowmourne
//    Shattering Charge             TODO
//    Ravenous Crusaders            TODO
//    Grave Robber                  TODO
// Blisterskin
//    Blistering Speed              TODO
//    Lords of the Burning Skies    TODO
//    Hellish Orator                TODO
// Gristlegore
//    Peerless Ferocity             TODO
//    Call to War                   TODO
//    Savage Strike                 TODO
//

} // namespace FleshEaterCourt

#endif // FLESHEATERCOURTS_H