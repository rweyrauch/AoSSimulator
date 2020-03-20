/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OGROIDMYRMIDON_H
#define OGROIDMYRMIDON_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class OgroidMyrmidon : public SlavesToDarknessBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    OgroidMyrmidon();
    ~OgroidMyrmidon() override = default;

    bool configure();

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    Weapon m_spearMissile,
        m_spear,
        m_horns;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Fury                      Yes
// Berserk Rage                     TODO
// Pit Marshall                     TODO
//

} // SlavesToDarkness

#endif //OGROIDMYRMIDON_H
