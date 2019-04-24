/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FYRESLAYER_H
#define FYRESLAYER_H

#include <Unit.h>
#include <Weapon.h>

namespace Fyreslayers
{

class Fyreslayer : public Unit
{
public:

    enum Lodge
    {
        None = 0,
        Vostarg,
        Greyfyrd,
        Hermdar,
        Lofnir
    };

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    Fyreslayer() = default;
    ~Fyreslayer() override = default;

    void setLodge(Lodge lodge);

protected:
    Fyreslayer(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    int chargeModifier() const override;
    int rollRunDistance() const override;

protected:

    Lodge m_lodge = None;
};

void Init();

} // namespace Fyreslayers

#endif //FYRESLAYER_H