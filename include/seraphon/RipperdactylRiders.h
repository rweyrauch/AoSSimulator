/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RIPPERDACTYLS_H
#define RIPPERDACTYLS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class RipperdactylRiders : public SeraphonBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    RipperdactylRiders();
    ~RipperdactylRiders() override = default;

    bool configure(int numModels);

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    void onRestore() override { m_usedToad = false; }

private:

    mutable bool m_usedToad = false;

    Weapon m_spear,
        m_spearAlpha,
        m_jaws;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Voracious Appetite               Yes
// Toad Rage                        Yes
//

} // namespace Seraphon

#endif //RIPPERDACTYLS_H