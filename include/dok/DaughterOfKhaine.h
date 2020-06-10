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

namespace DaughtersOfKhaine {

    enum Temple {
        Custom,
        HaggNar,
        DraichiGaneth,
        TheKraith,
        Khailebron
    };

    const std::array<int, 5> g_temple = {
        Custom,
        HaggNar,
        DraichiGaneth,
        TheKraith,
        Khailebron
    };

    class DaughterOfKhaine : public Unit {
    public:
        DaughterOfKhaine() = default;

        ~DaughterOfKhaine() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setTemple(Temple temple);

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

        Temple m_temple = Custom;
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