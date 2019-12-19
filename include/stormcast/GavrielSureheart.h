/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GAVRIELSUREHEART_H
#define GAVRIELSUREHEART_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class GavrielSureheart : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    GavrielSureheart();
    ~GavrielSureheart() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;

private:

    Weapon m_starboundBlade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Inescapable Vengeance            Yes
// Sigmarite Thundershield          Yes
// Once More, For Sigmar, Charge!   No
//

} // namespace StormcastEternals

#endif //GAVRIELSUREHEART_H