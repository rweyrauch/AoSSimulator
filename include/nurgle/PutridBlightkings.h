/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PUTRIDBLIGHTKINGS_H
#define PUTRIDBLIGHTKINGS_H

#include <Unit.h>
#include <Weapon.h>

namespace Nurgle
{

class PutridBlightkings : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 580;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    PutridBlightkings();
    ~PutridBlightkings() override = default;

    bool configure(int numModels, bool iconBearer, bool sonorousTocsin);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    int runModifier() const override;
    int chargeModifier() const override;

protected:

    bool m_iconBearer = false;
    bool m_sonorousTocsin = false;

private:

    Weapon m_blightedWeapon;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Virulent Discharge               No
// Blighted Weapons                 Yes
//

} // Nurgle

#endif //PUTRIDBLIGHTKINGS_H