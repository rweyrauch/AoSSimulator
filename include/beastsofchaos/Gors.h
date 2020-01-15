/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GORS_H
#define GORS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Gors : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 200;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    Gors();
    ~Gors() override = default;

    bool configure(int numModels, bool pairedBlades, bool brayhorn, bool bannerBearer);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    int toSaveModifier(const Weapon* weapon) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    bool m_pairedBlades = false;
    bool m_brayhorn = false;
    bool m_bannerBearer = false;

    Weapon m_gorBlade,
        m_gorBladeFoeRender;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Rend and Tear                    Yes
// Beastshields                     Yes
// Anarchy and Mayhem               Yes
//

} // namespace BeastsOfChaos

#endif //GORS_H