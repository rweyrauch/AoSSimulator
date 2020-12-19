/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BONESPLITTERZ_H
#define BONESPLITTERZ_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Bonesplitterz {

    enum class Warclan : int {
        None = 0,
        Bonegrinz,
        Drakkfoot,
        Icebone
    };

    enum class CommandTrait : int {
        None,

        // Prophet and Big Boss
        Killa_Instinkt,
        Waaagh_Monger,
        Great_Hunter,
        Power_Of_The_Beast,
        Voice_Of_Da_Gods,
        Monsta_Killa,

        // Wizard
        Dead_Kunnin,
        Master_Of_The_Weird,
        Fuelled_By_The_Spirits,

        // Clan specific
        A_Right_Monster, // Bonegrinz
        Purebred_War_Boar, // Icebone
        Fireball, // Drakkfoot
    };

    enum class Artefact : int {
        None,

        // Hero
        Dokk_Juice,
        Savage_Trophy,
        Lucky_Bone,
        Glowin_Tattooz,
        Greatdrake_Toof,
        Weepwood_Big_Shiv,

        // Wizard
        Big_Wurrgog_Mask,
        Morks_Boney_Bitz,
        Mystic_Waaagh_Paint,

        // Clan specific
        Maw_Krusha_Beast_Totem, // Bonegrinz
        Kattanak_Pelt, // Icebone
        Burnin_Tattooz, // Drakkfoot
    };

    enum class Lore : int {
        None,

        Squiggly_Curse,
        Breath_Of_Gorkamorka,
        Brutal_Beast_Spirits,
        Bone_Krusha,
        Kunnin_Beast_Spirits,
        Gorkamorkas_War_Cry,
    };

    class Bonesplitterz : public Unit {
    public:
        Bonesplitterz() = default;

        ~Bonesplitterz() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setWarclan(Warclan warclan);

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);

    protected:
        Bonesplitterz(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onStartCombat(PlayerId player) override;

        Wounds onEndCombat(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    protected:

        Warclan m_warclan = Warclan::None;
        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        bool m_stabStabStab = false;
        bool m_berserkStrength = false;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Tireless Trackers                TODO
// Warpaint                         Yes
// Monster Hunters                  Yes
//   Wild Abandon                   Yes
//   Stab! Stab! Stab!              Yes
//   Berserk Strength               Yes
// Spirit of the Beast              TODO
// Bonesplitterz Waaagh!            TODO
// Bonegrinz
//    Bring It On!                  TODO
//    Feel Da Spirit!               TODO
//    A Right Monster               TODO
// Icebone
//    Freezing Strike               Yes
//    Freeze and Run                TODO
//    Pure-bred War Boar            TODO
// Drakkfoot
//    Strength of Purpose           TODO
//    Shout Down da Magic!          TODO
//    Fireball                      TODO
//

    void Init();

} // namespace Bonesplitterz

#endif // BONESPLITTERZ_H