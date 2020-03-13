/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKARRBLOODWRATH_H
#define SKARRBLOODWRATH_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class SkarrBloodwrath : public KhorneBase
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SkarrBloodwrath();
    ~SkarrBloodwrath() override = default;

    bool configure();

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_blades;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Slaughterstorm                   Yes
// The Slaughterborn                TODO
// Murderous Paragon                TODO
//

} // namespace Khorne

#endif //SKARRBLOODWRATH_H