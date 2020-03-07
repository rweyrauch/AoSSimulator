/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TERRADONCHIEF_H
#define TERRADONCHIEF_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class TerradonChief : public SeraphonBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 70;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    TerradonChief();
    ~TerradonChief() override = default;

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
// Lead from on High                No
// Coordinated Attack               No
//

} // namespace Seraphon

#endif //TERRADONCHIEF_H