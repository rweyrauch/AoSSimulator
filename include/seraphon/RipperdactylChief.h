/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RIPPERDACTYLCHIEF_H
#define RIPPERDACTYLCHIEF_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class RipperdactylChief : public SeraphonBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    RipperdactylChief();
    ~RipperdactylChief() override = default;

    bool configure();

protected:


private:

    Weapon m_skyblade,
        m_jaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Voracious Appetite               No
// Ripperdactyl Assault             No
//

} // namespace Seraphon

#endif //RIPPERDACTYLCHIEF_H