/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OSSIARCHBASE_H
#define OSSIARCHBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    enum class Legion : int {
        None,
        Mortis_Praetorians,
        Petrifex_Elite,
        Stalliarch_Lords,
        Ivory_Host,
        Null_Myriad,
        Crematorians,
    };

    enum class CommandTrait : int {
        None,

        // Kavalos
        Ancient_Knowledge,
        Immortal_Ruler,
        Dark_Acolyte,
        Peerless_Warrior,
        Hatred_of_the_Living,
        Life_Stealer,

        // Mortisan
        // Ancient_Knowledge,
        // Immortal_Ruler,
        Dire_Ultimatum,
        Grave_Sand_Bones,
        Oathbreaker_Curse,
        Soul_Energy,

        // Legion specific
        Katakros_Chosen,    // Mortis Praetorians
        Mighty_Archaeossian,    // Petrifex Elite
        Twisted_Challenge,  // Stalliarch Lords
        Scrimshawed_Savage, // Ivory Host
        Unsettling_and_Sinister,    // Null Myriad
        Wrathful_Avenger    // Crematorians
    };

    enum class Artefact : int {
        None,

        // Kavaloi
        Mindblade,
        Lordly_Phylactery,
        Scroll_of_Command,
        Grave_Sand_Boneplate,
        Marrowpact,
        Helm_of_the_Ordained,

        // Boneshaper
        Artisans_Key,
        Lode_of_Saturation,
        The_Crafter_Gems,

        // Soulmason
        Gothizzar_Cartouche,
        Soul_Reservoir,
        Throne_of_Dzendt,

        // Soulreaper
        Luminscythe,
        Vial_of_Binding,
        Guardian_Reavesoul,

        // Legion specific
        Artificers_Blade,   // Mortis Praetorians
        Godbone_Armour,     // Petrifex Elite
        Nadir_Bound_Mount,  // Stalliarch Lords
        Beastbone_Blade,    // Ivory Host
        Baleful_Blade,      // Null Myriad
        Searing_Blade       // Crematorians
    };

    enum class Lore : int {
        Arcane_Command,
        Empower_Nadirite_Weapons,
        Protection_of_Nagash,
        Reinforce_Battle_Shields,
        Drain_Vitality,
        Mortal_Contract
    };

    class OssiarchBonereaperBase : public Unit {
    public:

        OssiarchBonereaperBase() = default;

        ~OssiarchBonereaperBase() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setLegion(Legion legion);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        OssiarchBonereaperBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Wounds applyWoundSave(const Wounds &wounds) override;
        Rerolls toSaveRerolls(const Weapon *weapon) const override;
        void onModelSlain(Wounds::Source source) override;
        bool battleshockRequired() const override;
        int theDreadLegion(const Unit *target);

    protected:

        Legion m_legion = Legion::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        lsignal::slot m_braveryBuffConnection;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deathless Warriors               Yes
// Ranks Unbroken by Dissent        Yes
// Relentless Discipline Points     TODO
// Unstoppable Advance              TODO
// Mortis Praetorians
//   The Dread Legion               Yes
//   Counter-strike                 TODO
//   Katakros's Chosen              TODO
// Petriflex Elite
//   Unstoppable Juggernauts        Yes
//   Bludgeon                       TODO
//   Mighty Archaossian             TODO
// Stalliarch Lord
//   Equumortoi                     Yes
//   Rally Back                     TODO
//   Twisted Challenge              TODO
// Ivory Host
//   Simmering Rage                 TODO
//   Tempur Fury                    TODO
//   Scrimshawed Savage             TODO
// Null Myriad
//   Eldritch Nulls                 TODO
//   Holdfast                       TODO
//   Unsettling and Sinister        TODO
// Crematorians
//   Immolation                     Yes
//   Levellers of Cities            TODO
//   Wrathful Avenger               TODO
//

    void Init();

} // namespace OssiarchBonereapers

#endif // OSSIARCHBASE_H