/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLACKKNIGHTS_H
#define BLACKKNIGHTS_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{

class BlackKnights : public LegionOfNagashBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BlackKnights();
    ~BlackKnights() override = default;

    bool configure(int numModels, bool standardBearers, bool hornblowers);

protected:

    int toWoundModifier(const Weapon* weapon, const Unit* target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    bool m_standardBearers = false;
    bool m_hornblowers = false;

    Weapon m_barrowLance,
        m_barrowLanceKnight,
        m_hoovesAndTeeth;

    static bool s_registered;

};

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  TODO
// Hornblower                       TODO
// Deathly Charge                   Yes
// Crypt Shields                    Yes
//


} //namespace Death

#endif //BLACKKNIGHTS_H
