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

namespace Ironjawz {

    enum class Warclan : int {
        None,
        Ironsunz,
        Bloodtoofs,
        Da_Choppas
    };

    enum class CommandTrait : int {
        None,

        // Megaboss
        Hulking_Muscle_Bound_Brute,     // Yes
        Live_To_Fight,                  // Yes
        Brutish_Cunning,                // TODO
        Ironclad,                       // Yes
        Bestial_Charisma,               // TODO
        Mighty_Waaagh,                  // TODO

        // Shaman
        Dead_Kunnin,                    // Yes
        Master_Of_The_Weird,            // Yes
        Bursting_With_Power,            // TODO

        // Clan specific
        Right_Fist_Of_Dakkbad, // Ironsunz
        Get_Da_Realmgate, // Bloodtoof TODO
        Checked_Out, // Da Choppas
    };

    enum class Artefact : int {
        None,

        // Megaboss
        Armour_Of_Gork,
        Destroyer,
        Daubing_Of_Mork,
        The_Golden_Toof,
        Metalrippas_Klaw,
        The_Boss_Skewer,

        // Shaman
        Great_Green_Visions,
        Amberbone_Hoard,
        Shamanic_Skullcape,

        // Clan specific
        Sunzblessed_Armour, // Ironsunz
        Quickduff_Amulet, // Bloodtoof
        Megaskull_Staff, // Da Choppas
    };

    enum class MountTrait : int {
        None,

        Big_Un,         // TODO
        Fast_Un,        // TODO
        Mean_Un,        // TODO
        Heavy_Un,       // TODO
        Loud_Un,        // TODO
        Weird_Un,       // TODO
    };

    enum class Warbeat : int {
        None,

        Get_Em_Beat,    // TODO
        Fixin_Beat,     // TODO
        Killa_Beat,     // TODO
    };

    enum class Lore : int {
        None,

        Brain_Bursta,
        Mighty_Eadbutt,
        Da_Blazin_Eyes,
        Da_Great_Big_Green_Hand_Of_Gork,
        Bash_Em_Ladz,
        Wrath_Of_Gork
    };

    class Ironjawz : public Unit {
    public:
        Ironjawz() = default;

        ~Ironjawz() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setWarclan(Warclan warclan);
        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        Ironjawz(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        int chargeModifier() const override;

        int runModifier() const override;

        void onBeginRound(int battleRound) override;

        void onCharged() override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int castingModifier() const override;

        int unbindingModifier() const override;

        int braveryModifier() const override;

    protected:

        Warclan m_warclan = Warclan::Ironsunz;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mighty Destroyers                TODO
// Smashing and Bashing             TODO
// Eager for Battle                 Yes
// Mad as Hell                      TODO
// Ironjawz Waaagh!                 TODO
// Ironsunz
//    Ironsunz Kunnin'              TODO
//    Alright - Get 'Em!            TODO
//    Right Fist of Dakkbad         Yes
// Bloodtoofs
//    Hunt and Crush                Yes
//    Break Through Da Line         TODO
//    Get Da Realmgate              TODO
// Da Choppas
//    Vandal Hordes                 TODO
//    Rabble Rouser                 TODO
//    Checked Out                   TODO
//

    void Init();

} // namespace Ironjawz
