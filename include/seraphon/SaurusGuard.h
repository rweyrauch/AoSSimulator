/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSGUARD_H
#define SAURUSGUARD_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusGuard : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    SaurusGuard();
    ~SaurusGuard() override = default;

    bool configure(int numModels, bool iconBearer, bool wardrum);

protected:

    Rerolls chargeRerolls() const override;

private:

    bool m_iconBearer = false;
    bool m_wardrum = false;

    Weapon m_celestitePolearm,
        m_celestitePolearmAlpha,
        m_jaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   No
// Wardrum                          Yes
// Selfless Protectors              No
//

} // namespace Seraphon

#endif //SAURUSGUARD_H