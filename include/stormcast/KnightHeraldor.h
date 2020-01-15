/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTHERALDOR_H
#define KNIGHTHERALDOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class KnightHeraldor : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels)  { return POINTS_PER_UNIT; }
    static void Init();

    KnightHeraldor();
    ~KnightHeraldor() override = default;

    bool configure();

protected:

private:

    Weapon m_broadsword;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Onwards to Glory                 No
// Thunderblast                     No
//

} // namespace StormcastEternals

#endif //KNIGHTHERALDOR_H