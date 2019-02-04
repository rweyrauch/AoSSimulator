/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFPLAGUES_H
#define LORDOFPLAGUES_H

#include <Unit.h>
#include <Weapon.h>

namespace Nurgle
{

class LordOfPlagues : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    LordOfPlagues();
    ~LordOfPlagues() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Hits applyHitModifiers(const Weapon *weapon, const Unit *unit, const Hits &hits) const override;

protected:


private:

    Weapon m_plagueriddenGreatBlade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Wanton Slaughter                 No
// Rotten Corpse Mulch              No
// Plagueridden Great Weapon        Yes
//

} // Nurgle

#endif //LORDOFPLAGUES_H