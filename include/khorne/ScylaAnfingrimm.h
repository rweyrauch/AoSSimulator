/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SCYLAANFINGRIMM_H
#define SCYLAANFINGRIMM_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class ScylaAnfingrimm : public KhorneBase
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ScylaAnfingrimm();
    ~ScylaAnfingrimm() override = default;

    bool configure();

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_brutalFists,
        m_serpentineTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brass Collar of Khorne           Yes
// Raging Fury                      Yes
// Bestial Leap                     Yes
//

} // namespace Khorne

#endif //SCYLAANFINGRIMM_H