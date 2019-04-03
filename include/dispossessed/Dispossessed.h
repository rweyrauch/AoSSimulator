/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DISPOSSESSED_H
#define DISPOSSESSED_H

#include <Unit.h>
#include <Weapon.h>

namespace Dispossessed
{

class Dispossessed : public Unit
{
public:
    enum Grudge
    {
        StuckUp,
        SpeedMerchants,
        MonstrousCheaters,
        CowardlyHorders,
        ShoddyCraftsmanship,
        SneakyAmbushers
    };

    Dispossessed() = default;
    ~Dispossessed() override = default;

protected:
    Dispossessed(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Grudge m_grudge = StuckUp;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stubborn to the End              Yes
// Grudgebound:
//      Stuck-up                    Yes
//      Speed Merchants             Yes
//      Monstrous Cheaters          Yes
//      Cowardly Hordes             Yes
//      Shoddy Craftsmanship        Yes
//      Sneaky Ambushers            No
//

void Init();

} // namespace Nighthaunt

#endif // DISPOSSESSED_H