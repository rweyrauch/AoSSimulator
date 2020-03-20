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
#include <Weapon.h>

namespace Sylvaneth {

    class SylvanethBase : public Unit {
    public:

        enum Glade {
            None = 0,
            Oakenbrow,
            Gnarlroot,
            Heartwood,
            Ironbark,
            Winterleaf,
            Dreadwood,
            Harvestboon
        };

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

        Glade m_glade = None;
    };

    void Init();

} // namespace Sylvaneth

#endif // SYLVANETHBASE_H