/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ICEFALLYHETEES_H
#define ICEFALLYHETEES_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class IcefallYhetees : public MawtribesBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    IcefallYhetees();
    ~IcefallYhetees() override = default;

    bool configure(int numModels);

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_clawsAndClubs;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Frost                    Yes
// Bounding Leaps                   Yes
// Invigorated by the Blizzard      TODO
//

} // namespace OgorMawtribes

#endif //ICEFALLYHETEES_H
