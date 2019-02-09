/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOONBOSS_H
#define LOONBOSS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class Loonboss : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 70;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Loonboss();
    ~Loonboss() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_moonslicer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Dead Tricksy                     Yes
// I'm Da Boss, Now Stab 'Em Good   No
//

} // namespace GloomspiteGitz

#endif //LOONBOSS_H