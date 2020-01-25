/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSWARSHRINE_H
#define CHAOSWARSHRINE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosWarshrine : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 170;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ChaosWarshrine();
    ~ChaosWarshrine() override = default;

    bool configure();

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_blade,
        m_fists;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    No
// Protection of the Dark Gods      No
// Favour of the Ruinous Powers     No
//   Favour of Khorne               No
//   Favour of Tzeentch             No
//   Favour of Nurgle               No
//   Favour of Slaanesh             No
//   Favour of Chaos                No
//

} // SlavesToDarkness

#endif //CHAOSWARSHRINE_H