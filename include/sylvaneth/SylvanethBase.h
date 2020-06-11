/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SYLVANETHBASE_H
#define SYLVANETHBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Sylvaneth {

    enum class Glade: int {
        None = 0,
        Oakenbrow,
        Gnarlroot,
        Heartwood,
        Ironbark,
        Winterleaf,
        Dreadwood,
        Harvestboon
    };

    const std::array<int, 8> g_glade = {
            (int)Glade::None,
            (int)Glade::Oakenbrow,
            (int)Glade::Gnarlroot,
            (int)Glade::Heartwood,
            (int)Glade::Ironbark,
            (int)Glade::Winterleaf,
            (int)Glade::Dreadwood,
            (int)Glade::Harvestboon
    };

    class SylvanethBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SylvanethBase() = default;

        ~SylvanethBase() override = default;

        void setGlade(Glade glade);

    protected:
        SylvanethBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls battleshockRerolls() const override;

        int braveryModifier() const override;

    protected:

        Glade m_glade = Glade::None;
    };

    void Init();

} // namespace Sylvaneth

#endif // SYLVANETHBASE_H