/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SORCERERONMANTICORE_H
#define SORCERERONMANTICORE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosSorcererOnManticore : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ChaosSorcererOnManticore();
    ~ChaosSorcererOnManticore() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_staff,
        m_fangsAndClaws,
        m_tail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Oracular Visions                 No
// Wind of Chaos                    No
// Territorial Predator             Yes
// Mark of Chaos                    No
//

} // SlavesToDarkness

#endif //SORCERERONMANTICORE_H