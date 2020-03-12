/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHIMERA_H
#define CHIMERA_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Chimera : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }

    Chimera();
    ~Chimera() override = default;

    bool configure();

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    int chargeModifier() const override;
    void onStartShooting(PlayerId player) override;

private:

    Weapon m_fieryBreath,
        m_avianHead,
        m_draconicHead,
        m_leonineHead,
        m_maulingClaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Draconic Head's Fiery Breath     Yes
// Vicious Charge                   Yes
//

} // namespace BeastsOfChaos

#endif //CHIMERA_H