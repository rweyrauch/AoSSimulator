/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDCELESTANT_H
#define LORDCELESTANT_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordCelestant : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    LordCelestant();
    ~LordCelestant() override = default;

    bool configure();

protected:

    int extraAttacks(const Weapon *weapon) const override
    {
        int attacks = Unit::extraAttacks(weapon);
        // Inescapable Vengeance
        if (m_charged) attacks += 1;
        return attacks;
    }
    void onStartShooting(PlayerId player) override;

private:

    static Weapon s_runeblade,
        s_warhammer;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Inescapable Vengeance            Yes
// Sigmarite Warcloak               Yes
// Furious Retribution              No
//

} // namespace StormcastEternals

#endif //LORDCELESTANT_H