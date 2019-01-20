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
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class DaughterOfKhaine : public Unit
{
public:
    DaughterOfKhaine() = default;
    ~DaughterOfKhaine() override = default;

protected:
    DaughterOfKhaine(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toWoundRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Rerolls runRerolls() const override;
    Rerolls chargeRerolls() const override;

    // TODO: implement Fanatical Faith, 6+ invulnerability save

    // TODO: implement the Unquenchable Fervour battleshock immunity.
};

} // namespace DaughtersOfKhaine

#endif //DAUGHTERSOFKHAINE_H