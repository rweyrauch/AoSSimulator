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
    ~SaurusGuard() override;

    bool configure(int numModels, bool iconBearer, bool wardrum);

protected:

    Rerolls chargeRerolls() const override;

    int stardrakeIcon(const Unit* target);

private:

    bool m_iconBearer = false;
    bool m_wardrum = false;

    Weapon m_celestitePolearm,
        m_celestitePolearmAlpha,
        m_jaws;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   Yes
// Wardrum                          Yes
// Selfless Protectors              TODO
//

} // namespace Seraphon

#endif //SAURUSGUARD_H