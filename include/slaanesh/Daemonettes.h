/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAEMONETTES_H
#define DAEMONETTES_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class Daemonettes : public SlaaneshBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Daemonettes();
    ~Daemonettes() override = default;

    bool configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower);

protected:

    Rerolls chargeRerolls() const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;

protected:

    bool m_iconBearer = false;
    bool m_bannerBearer = false;
    bool m_hornblower = false;

private:

    Weapon m_piercingClaws,
        m_piercingClawsAlluress;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearers                     Yes
// Banner Bearers                   Yes
// Hornblower                       No
// Lithe and Swift                  Yes
//

} // Slannesh

#endif //DAEMONETTES_H