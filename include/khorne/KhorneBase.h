/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHORNEBASE_H
#define KHORNEBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class KhorneBase : public Unit
{
public:

    enum SlaughterHost
    {
        None,
        ReapersOfVengeance,
        Bloodlords,
        Goretide,
        SkullfiendTribe
    };

    KhorneBase() = default;
    ~KhorneBase() override = default;

    void setSlaughterHost(SlaughterHost host);

protected:
    KhorneBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

protected:

    SlaughterHost m_slaughterHost = None;
};

void Init();

} // namespace Khorne

#endif // KHORNEBASE_H