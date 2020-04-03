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
#include <Weapon.h>

namespace Bonesplitterz {

    class Bonesplitterz : public Unit {
    public:
        Bonesplitterz() = default;

        ~Bonesplitterz() override = default;

        enum Warclan {
            Bonegrinz,
            Drakkfoot,
            Icebone
        };

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setWarclan(Warclan warclan);

    protected:
        Bonesplitterz(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Wounds applyWoundSave(const Wounds &wounds) override;

    protected:

        Warclan m_warclan = Bonegrinz;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Tireless Trackers                TODO
// Warpaint                         Yes
// Monster Hunters                  TODO
//   Wild Abandon                   TODO
//   Stab! Stab! Stab!              TODO
//   Berserk Strength               TODO
// Spirit of the Beast              TODO
// Bonesplitterz Waaagh!            TODO
//

    void Init();

} // namespace Bonesplitterz

#endif // BONESPLITTERZ_H