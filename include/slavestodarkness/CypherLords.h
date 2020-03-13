/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CYPHERLORDS_H
#define CYPHERLORDS_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class CypherLords : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 8;
    static const int MAX_UNIT_SIZE = 32;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    CypherLords();
    ~CypherLords() override = default;

    bool configure(int numModels);

protected:

    int chargeModifier() const override;
    void onWounded() override;

protected:

    bool m_hasThrallmaster = true;
    bool m_hasLuminate = true;

private:

    Weapon m_throwingStars,
        m_exoticBlades;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Luminate                         Yes
// Shattered Gloom Globe            TODO
//

} // SlavesToDarkness

#endif //CYPHERLORDS_H