/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RIPPERDACTYLS_H
#define RIPPERDACTYLS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class RipperdactylRiders : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 560;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    RipperdactylRiders();
    ~RipperdactylRiders() override = default;

    bool configure(int numModels);

protected:


private:

    Weapon m_spear,
        m_spearAlpha,
        m_claws,
        m_beak;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Swooping Dive                    No
// Star-bucklers                    No
// Voracious Appetite               No
// Toad Rage                        No
//

} // namespace Seraphon

#endif //RIPPERDACTYLS_H