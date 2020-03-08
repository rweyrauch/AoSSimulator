/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDKROAK_H
#define LORDKROAK_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class LordKroak : public SeraphonBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 320;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    LordKroak();
    ~LordKroak() override = default;

    bool configure();

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Wounds applyWoundSave(const Wounds &wounds) override;
    void onStartHero(PlayerId player) override;

private:

    Weapon m_barrier;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcane Vassal                    No
// Azyrite Force Barrier            Yes
// Dead for Innumerable Ages        Yes
// Impeccable Foresight             Yes
// Masters of Order                 No
// Celestial Deliverance            No
// Comet's Call                     No
// Gift from the Heavens            No
//

} // namespace Seraphon

#endif //LORDKROAK_H