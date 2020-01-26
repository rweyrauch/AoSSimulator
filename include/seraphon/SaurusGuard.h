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
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    SaurusGuard();
    ~SaurusGuard() override = default;

    bool configure(int numModels, bool iconBearer, bool wardrum);

protected:

    int braveryModifier() const override;
    int toSaveModifier(const Weapon *weapon) const override;

private:

    bool m_iconBearer = false;
    bool m_wardrum = false;

    Weapon m_celestitePolearm,
        m_celestitePolearmAlpha,
        m_jawsAndShield;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   No
// Wardrum                          No
// Stardrake Shields                Yes
// Sworn Guardians                  Yes
//

} // namespace Seraphon

#endif //SAURUSGUARD_H