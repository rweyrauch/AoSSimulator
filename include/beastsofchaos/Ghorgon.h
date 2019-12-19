/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GHORGON_H
#define GHORGON_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Ghorgon : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Ghorgon();
    ~Ghorgon() override = default;

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onStartCombat(PlayerId player) override;

private:

    Weapon m_butcheringBlades,
        m_hugeSlaveringMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ravenous Bloodgreed              Yes
// Swallow Whole                    Yes
//

} // namespace BeastsOfChaos

#endif //GHORGON_H