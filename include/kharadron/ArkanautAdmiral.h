/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKANAUTADMIRAL_H
#define ARKANAUTADMIRAL_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class ArkanautAdmiral : public KharadronBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ArkanautAdmiral();

    ~ArkanautAdmiral() override = default;

    bool configure();

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_pistol,
        m_skalfhammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// If You Want A Job Done...        Yes
// Protect the Admiral!             No
// Master of the Skies              No
// On My Mark, Fire!                No
// Repel Boarders!                  No
// Up And At Them!                  No
//

} // namespace KharadronOverlords

#endif //ARKANAUTADMIRAL_H
