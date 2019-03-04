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

namespace Skaven
{

class Skaventide : public Unit
{
public:
    Skaventide() = default;
    ~Skaventide() override = default;

protected:
    Skaventide(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
    int battlshockModifier() const override;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lead From the Back               No
// Scurry Away                      No
// Overwhelming Mass                Yes
// Strength in Numbers              Yes
// Skilled Manipulators             No
// Prized Creations                 No
// Masters of Murder                No
// Mighty Warlords                  No
// Warpstone Sparks                 No
// Echoes of the Great Plagues      No
//

} // namespace Skaven

#endif //SKAVENTIDE_H