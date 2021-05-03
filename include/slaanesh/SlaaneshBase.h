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

        Lurid_Haze_Invaders,
        Faultless_Blades_Pretenders,
        Scarlet_Cavalcade_Godseekers,
    };

    enum class CommandTrait : int {
        None,

        // Invaders
        Best_Of_The_Best,
        Glory_Hog,
        Hurler_Of_Obscenities,
        Territorial,
        Skin_Taker,
        Delusions_Of_Infallibility,

        // Pretenders
        Strength_Of_Godhood,        // TODO
        Monarch_Of_Lies,
        Craving_Stare,              // TODO
        Strongest_Alone,
        Hunter_Of_Godbeasts,
        Inspirer,

        // Godseekers
        Hunter_Supreme,
        Sweeping_Slash,
        Into_The_Fray,              // TODO
        Trail_Sniffer,              // TODO
        Symphoniac,                 // TODO
        Speed_Chaser,

        // Lurid Haze
        Feverish_Anticipation,

        // Faultless Blade
        Contest_Of_Cruelty,         // TODO

        // Scarlet Cavalcade
        Embodiment_Of_Haste,
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

        // Lurid Haze
        Oil_Of_Exultation,

        // Faultless Blades
        Contemptuous_Brand,

        // Scarlet Cavalcade
        Helm_Of_The_Last_Rider,
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

        SlaaneshBase() = delete;

        ~SlaaneshBase() override = default;

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

        int getDepravityPoints() const {
            return m_depravityPoints;
        }

        void clearDepravityPoints() {
            m_depravityPoints = 0;
        }

    protected:
        SlaaneshBase(Host host, const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
            setHost(host);
        }

        void setHost(Host host);

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int chargeModifier() const override;

        void onEndCharge(PlayerId player) override;

        void onRestore() override {
            Unit::onRestore();
            clearDepravityPoints();
        }

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        void onEndCombat(PlayerId player) override;

        void onStartCombat(PlayerId player) override;

        int woundModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls runRerolls() const override;

        Rerolls battleshockRerolls() const override;

        void onCharged() override;

        void onBeginRound(int battleRound) override;

        void onEndBattleshock(PlayerId player) override;

        void onEnemyModelSlainWithWeapon(int numSlain, Unit* enemyUnit, const Weapon* weapon, const Wounds& weaponDamage) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

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
//    Escalating Havoc              Yes
// Pretenders
//    Heir to the Throne            Yes
//    Warlord Supreme               Yes
// Godseekers
//    Thundering Cavalcade          Yes
//    Maniacal Hunters              Yes
// Lurid Haze
//    Billowing Mists               TODO
//    Intoxicating Pall             TODO
// Faultless Blades
//    Send Me Your Best             Yes
//    Armour of Arrogance           TODO
// Scarlet Cavalcade
//    Excessive Swiftness           TODO
//    Vicious Spurs                 TODO
//

    void Init();

} // namespace Slaanesh
