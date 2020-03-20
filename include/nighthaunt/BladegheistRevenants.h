/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEGHEISTREVENANTS_H
#define BLADEGHEISTREVENANTS_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class BladegheistRevenants : public Nighthaunt
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BladegheistRevenants();
    ~BladegheistRevenants() override = default;

    bool configure(int numModels);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_tombGreatblade;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Fearful Frenzy                   Yes
// Whirling Death                   Yes


} // namespace Nighthaunt

#endif // BLADEGHEISTREVENANTS_H
