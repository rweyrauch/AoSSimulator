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
#include <Weapon.h>

namespace Ironjawz
{

class Ironjawz : public Unit
{
public:
    Ironjawz() = default;
    ~Ironjawz() override = default;

    enum Warclan
    {
        Ironsunz,
        Bloodtoofs,
        DaChoppas
    };

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    void setWarclan(Warclan warclan);

protected:
    Ironjawz(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

protected:

    int chargeModifier() const override;
    int runModifier() const override;

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
//

void Init();

} // namespace Ironjawz

#endif // IRONJAWZ_H