/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONGOLEMS_H
#define IRONGOLEMS_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class IronGolems : public SlavesToDarknessBase
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

    IronGolems();
    ~IronGolems() override = default;

    bool configure(int numModels);

protected:

    int braveryModifier() const override;
    void onWounded() override;

protected:

    bool m_hasSignifer = true;

private:

    Weapon m_bolas,
        m_legionWeapons,
        m_legionWeaponsDominar;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Iron Resilience                  No
// Signifier                        Yes
//

} // SlavesToDarkness

#endif //IRONGOLEMS_H