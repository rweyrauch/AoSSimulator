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
    Fyreslayer() = default;
    ~Fyreslayer() override = default;

protected:
    Fyreslayer(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

};

void Init();

} // namespace Fyreslayers

#endif //FYRESLAYER_H