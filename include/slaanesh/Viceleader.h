/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VICELEADER_H
#define VICELEADER_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class ViceleaderHeraldOfSlaanesh : public SlaaneshBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ViceleaderHeraldOfSlaanesh();
    ~ViceleaderHeraldOfSlaanesh() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;

private:

    Weapon m_ravagingClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Lightning Reflexes               Yes
// Lithe and Swift                  Yes
// Acquiescence                     TODO
//

} // Slannesh

#endif //VICELEADER_H