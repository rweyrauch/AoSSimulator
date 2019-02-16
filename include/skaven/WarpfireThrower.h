/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARPFIRETHROWER_H
#define WARPFIRETHROWER_H

#include <Unit.h>
#include <Weapon.h>

namespace Skaven
{

class WarpfireThrower : public Unit
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 70;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    WarpfireThrower();
    ~WarpfireThrower() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int generateMortalWounds(const Unit *unit) override;

private:

    Weapon m_warpfireThrower,
        m_rustyKnives;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Warpfire                         Yes
// More-more Warpfire!              Yes
//

} // namespace Skaven

#endif //WARPFIRETHROWER_H