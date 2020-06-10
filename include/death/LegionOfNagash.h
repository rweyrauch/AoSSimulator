/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LEGIONOFNAGASH_H
#define LEGIONOFNAGASH_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Death {

    enum Legion {
        GrandHostOfNagash,
        LegionOfSacrament,
        LegionOfBlood,
        LegionOfNight
    };

    const std::array<int, 4> g_legion = {
            GrandHostOfNagash,
            LegionOfSacrament,
            LegionOfBlood,
            LegionOfNight
    };

    class LegionOfNagashBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        LegionOfNagashBase() = default;

        ~LegionOfNagashBase() override = default;

        void setLegion(Legion legion);

    protected:

        LegionOfNagashBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void deathlyInvocations(int numUnits, double range);

    protected:

        Legion m_legion = GrandHostOfNagash;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Unquiet Dead                 TODO
// Deathless Thralls                TODO
// The Bloodlines                   TODO
//    Dragon Warriors               TODO
//    Lords of Night                TODO
//    Necromantic                   TODO
//    Swift Death                   TODO
// Deathless Minons                 TODO
//

    void Init();

} // namespace Death

#endif //LEGIONOFNAGASH_H