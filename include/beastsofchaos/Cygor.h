/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CYGOR_H
#define CYGOR_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Cygor : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Cygor();
    ~Cygor() override = default;

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_desecratedBoulder,
        m_massiveHorns;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Soul-eater                       No
// Ghostsight                       Yes
//

} // namespace BeastsOfChaos

#endif //CYGOR_H