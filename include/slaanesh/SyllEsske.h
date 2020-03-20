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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
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