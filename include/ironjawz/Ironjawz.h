/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONJAWZ_H
#define IRONJAWZ_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Ironjawz {

    enum Warclan {
        Ironsunz,
        Bloodtoofs,
        DaChoppas
    };

    const std::array<int, 3> g_warclan = {
            Ironsunz,
            Bloodtoofs,
            DaChoppas
    };

    class Ironjawz : public Unit {
    public:
        Ironjawz() = default;

        ~Ironjawz() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setWarclan(Warclan warclan);

    protected:
        Ironjawz(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        int chargeModifier() const override;

        int runModifier() const override;

        void onBeginRound(int battleRound) override;

    protected:

        Warclan m_warclan = Ironsunz;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mighty Destroyers                TODO
// Smashing and Bashing             TODO
// Eager for Battle                 Yes
// Mad as Hell                      TODO
// Ironsunz Kunnin'                 TODO
// Alright - Get 'Em!               TODO
// Right Fist of Dakkbad            Yes
// Hunt and Crush                   Yes
// Break Through Da Line            TODO
// Get Da Realmgate                 TODO
// Vandal Hordes                    TODO
// Rabble Rouser                    TODO
// Checked Out                      TODO
//

    void Init();

} // namespace Ironjawz

#endif // IRONJAWZ_H