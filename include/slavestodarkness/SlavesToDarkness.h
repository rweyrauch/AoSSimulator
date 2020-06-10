/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAVESTODARKNESS_H
#define SLAVESTODARKNESS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    enum DamnedLegion {
        Ravagers,
        Cabalists,
        Despoilers,
        HostOfTheEverchosen
    };

    enum MarkOfChaos {
        Undivided,
        Nurgle,
        Khorne,
        Slaanesh,
        Tzeentch
    };

    const std::array<int, 4> g_damnedLegion = {
            Ravagers,
            Cabalists,
            Despoilers,
            HostOfTheEverchosen
    };
    const std::array<int, 5> g_markOfChaos = {
            Undivided,
            Nurgle,
            Khorne,
            Slaanesh,
            Tzeentch
    };

    class SlavesToDarknessBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SlavesToDarknessBase() = default;

        ~SlavesToDarknessBase() override = default;

        void setDamnedLegion(DamnedLegion legion);

        void setMarkOfChaos(MarkOfChaos mark);

    protected:

        SlavesToDarknessBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

    private:

        DamnedLegion m_legion = Ravagers;
        MarkOfChaos m_markOfChaos = Undivided;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Chaos
//    Aura of Khorne                Yes
//    Aura of Tzeentch              Partial/TODO
//    Aura of Nurgle                TODO
//    Aura of Slaanesh              TODO
//    Aura of Chaos Undivided       TODO
// Eye of the Gods                  TODO
// Ravagers                         TODO
// Cabalists                        TODO
// Despoilers                       TODO
// Host of the Everchosen           TODO

} // SlavesToDarkness

#endif //SLAVESTODARKNESS_H