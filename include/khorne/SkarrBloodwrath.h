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
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SkarrBloodwrath();
    ~SkarrBloodwrath() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_blades;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Slaughterstorm                   Yes
// The Slaughterborn                No
// Murderous Paragon                No
//

} // namespace Khorne

#endif //SKARRBLOODWRATH_H