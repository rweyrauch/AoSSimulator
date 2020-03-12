/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUECENSERBEARERS_H
#define PLAGUECENSERBEARERS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class PlagueCenserBearers : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    PlagueCenserBearers();
    ~PlagueCenserBearers() override = default;

    bool configure(int numModels);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls battleshockRerolls() const override;
    Wounds onEndCombat(PlayerId player) override;

private:

    Weapon m_censer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Frenzied Assault                 Yes
// Plague Disciples                 Yes
// Poisonous Fumes                  Yes
//

} // namespace Skaven

#endif //PLAGUECENSERBEARERS_H