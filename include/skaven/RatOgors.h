/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RATSOGORS_H
#define RATSOGORS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class RatOgors : public Skaventide
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    RatOgors();
    ~RatOgors() override = default;

    bool configure(int numModels, int numGuns);

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    Weapon m_gun,
        m_clawsBladesAndFangs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Rabid Fury                       Yes
//

} // namespace Skaven

#endif //RATSOGORS_H