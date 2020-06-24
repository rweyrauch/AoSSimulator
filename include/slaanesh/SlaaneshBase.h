/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAANESHBASE_H
#define SLAANESHBASE_H

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
        Best_of_the_Best,
        Glory_Hog,
        Hurler_of_Obscenities,
        Territorial,
        Skin_Taker,
        Extra_Wound,

        // Pretenders
        Strength_of_Godhood,
        Monarch_of_Lies,
        True_Child_of_Slaanesh,
        Strongest_Alone,
        Hunter_of_Godbeasts,
        Inspirer,

        // Godseekers
        Hunter_Supreme,
        Thrill_Seeker,
        Into_the_Fray,
        Trail_Sniffer,
        Symphoniac,
        Speed_Chaser,
    };

    enum class Artefact : int {
        None,

        // Invaders
        The_Rod_of_Misrule,
        Rapier_of_Ecstatic_Conquest,
        Whip_of_Subversion,
        Icon_of_Infinite_Excess,
        Fallacious_Gift,
        The_Beguiling_Gem,

        // Pretenders
        The_Crown_of_Dark_Secrets,
        Pendant_of_Slaanesh,
        Sliverslash,
        Sceptre_of_Domination,
        Breathtaker,
        Mask_of_Spiteful_Beauty,

        // Godseekers
        Cameo_of_the_Dark_Prince,
        Girdle_of_the_Realm_Racer,
        Threnody_Voicebox,
        Lash_of_Despair,
        Enrapturing_Circlet,
        Binding_of_Slaanesh,
    };

    enum class Lore : int {
        None,

        // Daemon
        Lash_of_Slaanesh,
        Pavane_of_Slaanesh,
        Hysterical_Frenzy,
        Soulslice_Shards,
        Phantasmagoria,
        Bord_of_Damnation,

        // Greater Daemon
        Song_of_Secrets,
        Progeny_of_Damnation,
        Slothful_Stupor,

        // Mortal
        Battle_Rapture,
        Dark_Delusions,
        Hellshriek,
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

    protected:
        SlaaneshBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int chargeModifier() const override;

    protected:

        Host m_host = Host::Godseekers;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Feast of Depravites              TODO
// Locus of Diversion               TODO
// Euphoric Killers                 Yes
// Invaders
//    Figureheads of the Dark Prince    TODO
//    Escalating Havoc              TODO
// Pretenders
//    Heir to the Throne            Yes
//    Warlord Supreme               TODO
// Godseekers
//    Thundering Cavalcade          Yes
//    Maniacal Hunters              TODO
//

    void Init();

} // namespace Slaanesh

#endif //SLAANESHBASE_H