/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HELLPITABOMINATION_H
#define HELLPITABOMINATION_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class HellPitAbomination : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    HellPitAbomination();
    ~HellPitAbomination() override = default;

    bool configure();

protected:

    void onStartHero(PlayerId player) override;
    int generateMortalWounds(const Unit *unit) override;
    void onSlain() override;

    int getDamageTableIndex() const;

protected:

    bool m_beenSlain = false;

private:

    Weapon m_gnashingTeath,
        m_flailingFists,
        m_avalancheOfFlesh;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Avalanche of Flesh               Yes
// Regenerating Monstrosity         Yes
// Terrifying                       No
// Warpstone Spikes                 No
// Too Horrible to Die              Yes
//

} // namespace Skaven

#endif //HELLPITABOMINATION_H