/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LADYOLYNDER_H
#define LADYOLYNDER_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class LadyOlynder : public Nighthaunt
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    LadyOlynder();
    ~LadyOlynder() override = default;

    bool configure();

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartHero(PlayerId player) override;
    void onStartShooting(PlayerId player) override;
    void onRestore() override;

private:

    bool m_graveSandsOfTimeUsed = false;

    Weapon m_staff,
        m_claws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// Grave-sands of Time              Yes
// Lifting the Veil                 Yes
// Mortarch of Grief                TODO
// Wail of the Damned               Yes
// No Rest For the Wicked           TODO
//


} // namespace Nighthaunt

#endif // LADYOLYNDER_H
