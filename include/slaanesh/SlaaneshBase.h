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

namespace Slaanesh {

    enum class Host : int {
        Invaders,
        Pretenders,
        Godseekers,
    };

    enum class CommandTrait : int {
        None,

        // Invaders
        Best_Of_The_Best,
        Glory_Hog,
        Hurler_Of_Obscenities,
        Territorial,
        Skin_Taker,
        Extra_Wound,

        // Pretenders
        Strength_Of_Godhood,
        Monarch_Of_Lies,
        True_Child_Of_Slaanesh,
        Strongest_Alone,
        Hunter_Of_Godbeasts,
        Inspirer,

        // Godseekers
        Hunter_Supreme,
        Thrill_Seeker,
        Into_The_Fray,
        Trail_Sniffer,
        Symphoniac,
        Speed_Chaser,
    };

    enum class Artefact : int {
        None,

        // Invaders
        The_Rod_Of_Misrule,
        Rapier_Of_Ecstatic_Conquest,
        Whip_Of_Subversion,
        Icon_Of_Infinite_Excess,
        Fallacious_Gift,
        The_Beguiling_Gem,

        // Pretenders
        The_Crown_Of_Dark_Secrets,
        Pendant_Of_Slaanesh,
        Sliverslash,
        Sceptre_Of_Domination,
        Breathtaker,
        Mask_Of_Spiteful_Beauty,

        // Godseekers
        Cameo_Of_The_Dark_Prince,
        Girdle_Of_The_Realm_Racer,
        Threnody_Voicebox,
        Lash_Of_Despair,
        Enrapturing_Circlet,
        Binding_Of_Slaanesh,
    };

    enum class Lore : int {
        None,

        // Daemon
        Lash_Of_Slaanesh,
        Pavane_Of_Slaanesh,
        Hysterical_Frenzy,
        Soulslice_Shards,
        Phantasmagoria,
        Born_Of_Damnation,

        // Greater Daemon
        Paths_Of_The_Dark_Prince,
        Progeny_Of_Damnation,
        Slothful_Stupor,

        // Mortal
        Battle_Rapture,
        Judgement_Of_Excess,
        Dark_Delusions,
    };

    class SlaaneshBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SlaaneshBase() = default;

        ~SlaaneshBase() override = default;

        void setHost(Host host);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

        int getDepravityPoints() const {
            return m_depravityPoints;
        }
        void clearDepravityPoints() {
            m_depravityPoints = 0;
        }

    protected:
        SlaaneshBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int chargeModifier() const override;

        void onEndCharge(PlayerId player) override;

        void onRestore() override {
            Unit::onRestore();
            clearDepravityPoints();
        }

    protected:

        Host m_host = Host::Godseekers;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        int m_depravityPoints = 0;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Feast of Depravities             TODO
// Locus of Diversion               Yes
// Euphoric Killers                 Yes
// Invaders
//    Figureheads of the Dark Prince    TODO
//    Escalating Havoc              TODO
//    Best of the Best              TODO
//    Glory Hog                     TODO
//    Hurler of Obscenities         TODO
//    Territorial                   TODO
//    Skin-taker                    TODO
//    Delusions of Infallibility    TODO
// Pretenders
//    Heir to the Throne            Yes
//    Warlord Supreme               TODO
//    Strength of Godhood           TODO
//    Monarch of Lies               TODO
//    Craving Stare                 TODO
//    Strongest Alone               TODO
//    Hunter of Godbeasts           TODO
//    Inspirer                      TODO
// Godseekers
//    Thundering Cavalcade          Yes
//    Maniacal Hunters              TODO
//    Hunter Supreme                TODO
//    Sweeping Slash                TODO
//    Into the Fray                 TODO
//    Trail-sniffer                 TODO
//    Symphoniac                    TODO
//    Speed-chaser                  TODO
//

    void Init();

} // namespace Slaanesh
