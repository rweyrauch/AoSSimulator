/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODKNIGHTS_H
#define BLOODKNIGHTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class BloodKnights : public Unit
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 240;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    BloodKnights();
    ~BloodKnights() override = default;

    bool configure(int numModels, bool standardBearers, bool hornblowers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int damageModifier(const Weapon* weapon, const Unit* target, const Dice::RollResult& woundRolls) const override;
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
// The Hunger                       No
// Martial Fury                     Yes
// Bloodshields                     Yes
//

} //namespace Death

#endif //BLOODKNIGHTS_H
