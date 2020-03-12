/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEPRIESTONPLAGUEFURNACE_H
#define PLAGUEPRIESTONPLAGUEFURNACE_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class PlaguePriestOnPlagueFurnace : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 200;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    PlaguePriestOnPlagueFurnace();

    ~PlaguePriestOnPlagueFurnace() override = default;

    bool configure();

protected:

private:

    Weapon m_censer,
        m_staff,
        m_blades,
        m_spikes;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Altar of the Horned Rat
// Great Plague Censer
// Noxious Prayers
//   Filth-filth!
//   Rabid-rabid!
// Poisonous Fumes
// Protection of the Horned Rat
// Pushed into Battle
//

} // namespace Skaven

#endif //PLAGUEPRIESTONPLAGUEFURNACE_H