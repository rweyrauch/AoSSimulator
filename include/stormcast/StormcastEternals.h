/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMCASTETERNALS_H
#define STORMCASTETERNALS_H

#include <Unit.h>
#include <Weapon.h>

namespace StormcastEternals
{

class StormcastEternal : public Unit
{
public:
    StormcastEternal() = default;
    ~StormcastEternal() override = default;

protected:
    StormcastEternal(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int braveryModifier() const override;
    Rerolls battleshockRerolls() const override;

};

void Init();

} // namespace StormcastEternals

#endif //STORMCASTETERNALS_H