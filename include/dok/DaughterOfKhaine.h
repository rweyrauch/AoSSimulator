/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAUGHTERSOFKHAINE_H
#define DAUGHTERSOFKHAINE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace DaughtersOfKhaine {

    enum class Temple : int {
        None = 0,
        HaggNar,
        DraichiGaneth,
        TheKraith,
        Khailebron
    };

    enum class CommandTrait : int {
        None,

        Bathed_in_Blood,
        Zealous_Orator,
        Bloody_Sacrificer,
        Terrifying_Beauty,
        Mistress_of_Poisons,
        True_Believer,

        // Temple specific
        Devoted_Disciples,  // Hagg Nar
        Murder_of_Illusion, // Khailebron
    };

    enum class Artefact : int {
        None,

        // Hero
        Crown_of_Woe,
        Cursed_Blade,
        Amulet_of_Dark_Fire,
        Crone_Blade,
        Thousand_and_One_Dark_Blessings,
        Bloodbane_Venom,

        // Wizard
        Shadow_Stone,
        Rune_of_Ulgu,
        The_Mirror_Glaive,
        Seven_Fold_Shadow,
        Crystal_Heart,
        Shade_Claw,

        // Priest
        Blood_Sigil,
        Iron_Circlet,
        Rune_of_Khaine,
        Crimson_Shard,
        Khainite_Pendant,
        Hagbrew,

        // Temple specific
        The_Darksword,  // Draichi Ganeth
        Venom_of_Nagendra,  // The Kraith
    };

    enum class Lore : int {
        None,

        Steed_of_Shadows,
        Pit_of_Shades,
        Mirror_Dance,
        The_Withering,
        Mindrazor,
        Shroud_of_Despair,
    };

    enum class Prayer : int {
        None,

        Catechism_of_Murder,
        Blessing_of_Khaine,
        Martyrs_Sacrifice,
        Crimson_Rejuvination,
        Covenant_of_the_Iron_Heart,
        Sacrament_of_Blood,
    };

    class DaughterOfKhaine : public Unit {
    public:
        DaughterOfKhaine() = default;

        ~DaughterOfKhaine() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setTemple(Temple temple);

        void setCommandTrait(CommandTrait trait);

        void setArtefact(Artefact artefact);

    protected:
        DaughterOfKhaine(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

        Rerolls runRerolls() const override;

        Rerolls chargeRerolls() const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        Wounds applyWoundSave(const Wounds &wounds) override;

    protected:

        Temple m_temple = Temple::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fanatical Faith                  Yes
// Blood Rites                      Yes
//    Quickening Bloodlust          Yes
//    Headlong Fury                 Yes
//    Zealot's Rage                 Yes
//    Slaughter's Strength          Yes
//    Unquenchable Fervour          Yes
// Daughters of the First Temple    Yes
// Bladed Killers                   Yes
// Disciples of Slaughter           TODO
// Concealment and Stealth          Yes
//

    void Init();

} // namespace DaughtersOfKhaine

#endif //DAUGHTERSOFKHAINE_H