/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKAVENTIDE_H
#define SKAVENTIDE_H

#include <Unit.h>
#include <Weapon.h>

namespace Skaven {

    class Skaventide : public Unit {
    public:
        Skaventide() = default;

        ~Skaventide() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

    protected:
        Skaventide(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int braveryModifier() const override;

        bool moreMore() const;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lead From the Back               TODO
// Scurry Away                      TODO
// Overwhelming Mass                Yes
// Strength in Numbers              Yes
// Skilled Manipulators             TODO
// Prized Creations                 TODO
// Masters of Murder                TODO
// Mighty Warlords                  TODO
// Warpstone Sparks                 TODO
// Echoes of the Great Plagues      TODO
//

    void Init();

} // namespace Skaven

#endif //SKAVENTIDE_H