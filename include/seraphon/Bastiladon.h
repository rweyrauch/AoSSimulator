/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BASTILADON_H
#define BASTILADON_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Bastiladon : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    Bastiladon();
    ~Bastiladon() override = default;

    bool configure();

protected:

private:

    Weapon m_beam,
        m_javelins,
        m_tail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Impervious Defence               No
// Light of the Heavens             No
// Tide of Snakes                   No
//

} // namespace Seraphon

#endif //BASTILADON_H