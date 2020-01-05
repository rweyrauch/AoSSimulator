/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WIGHTKINGAXE_H
#define WIGHTKINGAXE_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class WightKingWithBlackAxe : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    WightKingWithBlackAxe();
    ~WightKingWithBlackAxe() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_blackAxe;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Black Axe                        No
// Barrow Armour                    Yes
// Deathly Invocation               No
// Lord of Bones                    No
//

} // namespace Death

#endif // WIGHTKINGAXE_H