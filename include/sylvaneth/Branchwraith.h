/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BRANCHWRAITH_H
#define BRANCHWRAITH_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Branchwraith : public SylvanethBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Branchwraith();
    ~Branchwraith() override = default;

    bool configure();

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_piercingTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Blessing of the Forest           Yes
// Roused to Wrath                  No
//

} // namespace Sylvaneth

#endif //BRANCHWRAITH_H