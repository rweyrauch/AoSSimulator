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

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

private:

    Weapon m_blade,
        m_fists;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Protection of the Dark Gods      TODO
// Favour of the Ruinous Powers     TODO
//   Favour of Khorne               TODO
//   Favour of Tzeentch             TODO
//   Favour of Nurgle               TODO
//   Favour of Slaanesh             TODO
//   Favour of Chaos                TODO
//

} // SlavesToDarkness

#endif //CHAOSWARSHRINE_H