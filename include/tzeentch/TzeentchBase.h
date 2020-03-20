/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZEENTCHBASE_H
#define TZEENTCHBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch {

    class TzeentchBase : public Unit {
    public:

        enum ChangeCoven {
            None,
            EternalConflagration,
            HostsDuplicitous,
            HostsArcanum,
            CultOfTheTransientForm,
            PyrofaneCult,
            GuildOfSummoners
        };

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        TzeentchBase() = default;

        ~TzeentchBase() override = default;

        void setChangeCoven(ChangeCoven coven);

    protected:

        TzeentchBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        ChangeCoven m_coven = None;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Locus of Change                  Yes
// Twisters of Materiality          Yes
// Ranks of Mischievous Mirages     TODO
// Thieves of all Things Arcane     TODO
// The Change-gift                  TODO
// Arrows of Tzeentch               Partial
// Scions of the Exiled             TODO
//

} // Tzeentch

#endif //TZEENTCHBASE_H