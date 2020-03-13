/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SYLLESSKE_H
#define SYLLESSKE_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class SyllEsske : public SlaaneshBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SyllEsske();
    ~SyllEsske() override = default;

    bool configure();

protected:

private:

    Weapon m_axeOfDominion,
        m_scourgingWhip;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Symbiosis                 TODO
// Lithe and Swift                  Yes
// Subvert                          TODO
// Regal Authority                  TODO
//

} // Slannesh

#endif //SYLLESSKE_H