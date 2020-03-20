/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODSTOKER_H
#define BLOODSTOKER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodstoker : public KhorneBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Bloodstoker();
    ~Bloodstoker() override = default;

    bool configure();

protected:

private:

    Weapon m_tortureBlade,
        m_bloodWhip;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Whipped to Fury                  TODO
//

} // namespace Khorne

#endif //BLOODSTOKER_H