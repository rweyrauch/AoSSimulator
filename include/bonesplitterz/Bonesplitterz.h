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

    enum Warclan {
        Bonegrinz,
        Drakkfoot,
        Icebone
    };

    const std::array<int, 3> g_warclan = {
            Bonegrinz, Drakkfoot, Icebone
    };

    class Bonesplitterz : public Unit {
    public:
        Bonesplitterz() = default;

        ~Bonesplitterz() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setWarclan(Warclan warclan);

    protected:
        Bonesplitterz(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Wounds applyWoundSave(const Wounds &wounds) override;

        void onStartCombat(PlayerId player) override;

        Wounds onEndCombat(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    protected:

        Warclan m_warclan = Bonegrinz;

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
// Bring It On!                     TODO
// Feel Da Spirit!                  TODO
// A Right Monster                  TODO
// Freezing Strike                  Yes
// Freeze and Run                   TODO
// Pure-bred War Boar               TODO
// Strength of Purpose              TODO
// Shout Down da Magic!             TODO
// Fireball                         TODO
//

    void Init();

} // namespace Bonesplitterz

#endif // BONESPLITTERZ_H