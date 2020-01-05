/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CENTIGORS_H
#define CENTIGORS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Centigors : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Centigors();
    ~Centigors() override = default;

    bool configure(int numModels, bool brayhorn, bool bannerBearer);

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;
    int runModifier() const override;
    int toSaveModifier(const Weapon *weapon) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_brayhorn = false;
    bool m_bannerBearer = false;
    bool m_drunkenRevelry = false;

    Weapon m_centigorSpear,
        m_centigorSpearGorehoof;
    Weapon m_clawedForelimbs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brayhorn                         Yes
// Banner Bearer                    Yes
// Beastbucklers                    Yes
// Charging Spear                   Yes
// Drunken Revelry                  Yes
//

} // namespace BeastsOfChaos

#endif //CENTIGORS_H