/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef SAVAGEBIGBOSS_H
#define SAVAGEBIGBOSS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{
class SavageBigBoss : public Bonesplitterz
{
public:

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    SavageBigBoss();
    ~SavageBigBoss() override = default;

    bool configure();

protected:


private:

    Weapon m_chompasBoss;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Let Me at 'Em                    TODO
// Savage Attack                    TODO
//

} // namespace Bonesplitterz

#endif //SAVAGEBIGBOSS_H