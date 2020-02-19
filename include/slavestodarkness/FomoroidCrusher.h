/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FOMOROIDCRUSHER_H
#define FOMOROIDCRUSHER_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class FomoroidCrusher : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    FomoroidCrusher();
    ~FomoroidCrusher() override = default;

    bool configure();

protected:

    void onCharged() override;
    int rollChargeDistance() const override;
    void onStartHero(PlayerId player) override;

private:

    Weapon m_hurledTerrain,
        m_fists;
    mutable int m_lastChargeDistance = 0;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Rampage                          Yes
// Insurmountable Strength          No
//

} // SlavesToDarkness

#endif //FOMOROIDCRUSHER_H
