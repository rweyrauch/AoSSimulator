/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKSHAMAN_H
#define ORRUKSHAMAN_H

#include <Unit.h>
#include <Weapon.h>

namespace Greenskinz
{

class OrrukGreatShaman : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    OrrukGreatShaman();
    ~OrrukGreatShaman() override = default;

    bool configure(bool warboar);

protected:

    int castingModifier() const override;

private:

    Weapon m_totemicStaff,
        m_boarsTusks;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Waaagh! Energy                   Yes
// Gaze of Mork                     TODO

} // namespace Greenskinz

#endif // ORRUKSHAMAN_H