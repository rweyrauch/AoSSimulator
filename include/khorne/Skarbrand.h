/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKARBRAND_H
#define SKARBRAND_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Skarbrand : public Unit
{
public:
    static const int BASESIZE = 100;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 400;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Skarbrand();
    ~Skarbrand() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    Rerolls chargeRerolls() const override;
    int generateMortalWounds(const Unit *unit) override;
    void onStartShooting(PlayerId player) override;

private:

    bool m_attackedInPreviousRound = false;

    Weapon m_slaughter,
        m_carnage;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Skarbrand's Rage                 No
// Roar of Total Rage               Yes
// Total Carnage                    Yes
// Inescapable Wrath                Yes
//

} // namespace Khorne

#endif //SKARBRAND_H