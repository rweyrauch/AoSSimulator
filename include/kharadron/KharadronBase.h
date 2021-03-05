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

namespace KharadronOverlords {

    enum class Skyport : int {
        None = 0,
        Barak_Nar,
        Barak_Zilfin,
        Barak_Zon,
        Barak_Urbaz,
        Barak_Mhornar,
        Barak_Thryng
    };

    enum class Artycle : int {
        None,

        Honour_Is_Everything,
        Master_The_Skies,
        Settle_The_Grudges,

        // Skyport specific
        Respect_Your_Commanders, // Barak-Nar
        //Master_the_Skies, // Barak-Zilfin
        //Honour_is_Everything, // Barak-Zon
        Seek_New_Prospects, // Barak-Urbaz
        //Seek_New_Prospects, // Barak-Mhornar
        Chronicle_Of_Grudges, // Barak-Thryng
    };

    enum class Amendment : int {
        None,

        Always_Take_What_You_Are_Owed,
        Prosecute_Wars_With_All_Haste,
        Trust_To_Your_Guns,

        // Skyport specific
        Trust_Aethermatics_Not_Superstition, // Barak-Nar
        Dont_Argue_With_The_Wind, // Barak-Zilpin
        Leave_No_Duardin_Behind, // Barak-Zon
        //Always_Take_What_You_Are_Owed, // Barak-Urbaz
        //Prosecute_Wars_With_All_Haste, // Barak-Mhornar
        Take_Help_Where_You_Can_Get_It, // Barak-Thryng
    };

    enum class Footnote : int {
        None,

        Theres_No_Reward_Without_Risk,  //
        Theres_No_Trading_With_Some_People,
        Without_Our_Ships_We_Are_Naught,

        // Skyport specific
        Through_Knowledge_Power, // Barak-Nar
        Theres_Always_A_Breeze, // Barak-Zilfin
        Show_Them_Your_Steel,// Barak-Zon
        Where_Theres_War_Theres_Gold, // Barak-Urbaz
        Who_Strikes_First_Strikes_Hardest, // Barak-Mhornar
        Honour_The_Gods_Just_In_Case, // Barak-Thryng
    };

    enum class CommandTrait : int  {
        None,

        // Admiral
        Wealthy,                    // Yes
        Tough_As_Old_Boots,         // Yes
        Grudgebearer,               // TODO
        Cunning_Fleetmaster,        // TODO
        War_Wounds,                 // TODO
        A_Scholar_And_An_Arkanaut,  // TODO

        // Endrinmaster
        // Wealthy,
        // Tough_as_Old_Boots,
        // Grudgebearer,
        Grandmaster,                // TODO
        Great_Tinkerer,             // TODO
        Endrinprofessor,            // TODO

        // Navigatgor
        // Wealthy,
        // Tough_as_Old_Boots,
        Stormcaller,                // TODO
        Ride_The_Winds,             // TODO
        Sceptic,                    // TODO
        Diviner,                    // TODO

        // Khemist
        // Wealthy,
        // Tough_as_Old_Boots,
        // Grudgebearer,
        A_Nose_For_Gold,            // TODO
        Genius_In_The_Making,       // TODO
        Collector,                  // TODO

        // Skyport
        Champion_Of_Progress,       // Barak-Nar TODO
        Master_Commander,           // Barak-Zilfin TODO
        Bearer_Of_The_Ironstar,     // Barak-Zon TODO
        Khemist_Supreme,            // Barak-Urbaz TODO
        Opportunistic_Privateer,    // Barak-Mhornar TODO
        Supremely_Stubborn,         // Barak-Thryng TODO
    };

    enum class Artefact : int {
        None,

        // Admiral
        Masterwrought_Armour,
        Hammer_Of_Aetheric_Might,
        Gattlesons_Endless_Repeater,
        Rune_Of_Mark,
        Flask_Of_Vintage_Amberwhisky,
        Proclamtor_Mask_Hailer,

        // Navigator
        Cyclonic_Aethometer,
        Svaregg_Stein_Illuminator_Flarepistol,
        Voidstone_Orb,

        // Endrinmaster
        Cogmonculus,
        Aetherquartz_Monolens,
        Seismic_Shock_Gauntlets,

        // Endrinmaster w/Dirigible
        Aether_Injection_Boosters,
        Phosphorite_Bomblets,
        Miniaturized_Aethermatic_Repulsion_Field,

        // Khemist
        Emergency_Ventplates,
        Caustic_Anatomiser,
        Spell_In_A_Bottle,

        // Skyport
        Aethercharged_Rune, // Barak-Nar
        Staff_Of_Ocular_Optimisation, // Barak-Zilfin
        Aethersped_Hammer, // Barak-Zon
        Breath_Of_Morgrim, // Barak-Urbaz
        Galeforce_Stave, // Barak-Mhornar
        Grudgehammer, // Barak-Thryng
    };

    enum class Endrinwork {
        None,

        // Ironclad
        The_Last_Word,              // TODO
        Hegsson_Solutions_Old_Reliable_Hullplates, // Yes
        Ebullient_Buoyancy_Aid,     // TODO
        Prudency_Chutes,            // TODO
        Magnificent_Omniscope,      // Yes
        Zonbarcorp_Dealbreaker_Battle_Ram, // TODO

        // Frigate
        //Prudency_Chutes,
        //Magnificent_Omniscope,
        Malefic_Skymines,           // TODO

        // Gunhauler
        Iggrind_Kaz_Surge_Injection_Endrin_Mk_Iv, // TODO
        Zonbarcorp_Debtsettler_Spar_Torpedo, // TODO
        Coalbeards_Collapsible_Compartments, // TODO
    };

    class KharadronBase : public Unit {
    public:

        KharadronBase() = default;

        ~KharadronBase() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setSkyport(Skyport skyport);

        void setCode(Artycle artycle, Amendment amendment, Footnote footnote);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);
        void setEndrinwork(Endrinwork endrinwork);

    protected:
        KharadronBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int braveryModifier() const override;

        void onRestore() override;

        void onBeginRound(int battleRound) override;

        int woundModifier() const override;

        int moveModifier() const override;

        Rerolls chargeRerolls() const override;

        void onEndShooting(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

    protected:

        Skyport m_skyport = Skyport::None;

        // Code for Custom Skyport
        Artycle m_artycle = Artycle::None;
        Amendment m_amendment = Amendment::None;
        Footnote m_footnote = Footnote::None;

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
        Endrinwork m_endrinwork = Endrinwork::None;

        static bool s_usedFootnote;
        int m_aetherGold = 1;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aether-Gold                      TODO
// Stick to the Code
// Artycle
//   Honour is Everything           Yes
//   Master the Skies               Yes
//   Settle the Grudges             TODO
//   Respect Your Commanders        TODO
//   Seek New Prospects             TODO
//   Chronicle of Grudges           TODO
// Ammendment
//   Always Take What You Are Owed  TODO
//   Prosecute Wars With All Haste  TODO
//   Trust To Your Guns             TODO
//   Trust Aethermatics not Superstition    TODO
//   Dont Argue With the Wind       TODO
//   Leave No Duardin Behind        TODO
//   Take Help Where You Can Get It TODO
// Footnote
//   There's No Reward Without Risk Yes
//   There's No Trading With Some People    TODO
//   Without Our Ships, We Are Naught   Yes
//   Through Knowledge Power        TODO
//   Theres Always a Breeze         TODO
//   Show Them_Your_Steel           TODO
//   Where Theres War Theres Gold   TODO
//   Who Strikes First Strikes Hardest  TODO
//   Honour the Gods Just in Case   TODO
//

    void Init();

} // namespace KharadronOverlords
