/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODKNIGHTS_H
#define BLOODKNIGHTS_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{

class BloodKnights : public LegionOfNagashBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BloodKnights();
    ~BloodKnights() override = default;

    bool configure(int numModels, bool standardBearers, bool hornblowers);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    bool m_standardBearers = false;
    bool m_hornblowers = false;

    Weapon m_templarLanceOrBlade,
        m_templarLanceOrBladeKastellan,
        m_hoovesAndTeeth;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  No
// Hornblower                       No
// The Hunger                       No
// Martial Fury                     Yes
// Bloodshields                     Yes
//

} //namespace Death

#endif //BLOODKNIGHTS_H
