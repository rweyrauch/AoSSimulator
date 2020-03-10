/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RAZORDONS_H
#define RAZORDONS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Razordons : public SeraphonBase
{
public:

    static const int BASESIZE = 26;
    static const int BASESIZE_RAZORDON = 60; // x35 oval
    static const int WOUNDS = 1;
    static const int WOUNDS_RAZORDON = 3;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Razordons();
    ~Razordons() override = default;

    bool configure(int numModels);

protected:

    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_spikes,
        m_tail,
        m_goad;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Piercing Barbs                   Yes
// Instinctive Defense              No
//

} // namespace Seraphon

#endif //RAZORDONS_H