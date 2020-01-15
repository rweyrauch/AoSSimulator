/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef ORRUKARROWBOYS_H
#define ORRUKARROWBOYS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{

class SavageOrrukArrowboys : public Bonesplitterz
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    SavageOrrukArrowboys();
    ~SavageOrrukArrowboys() override = default;

    bool configure(int numModels, bool skullThumper, bool totemBearer);

protected:

    int chargeModifier() const override
    {
        int mod = Unit::chargeModifier();
        if (m_thumper)
        {
            mod += 2;
        }
        return mod;
    }

    int braveryModifier() const override
    {
        int mod = Unit::braveryModifier();
        if (m_totemBearer)
        {
            mod++;
        }
        return mod;
    }

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

private:

    bool m_thumper = false;
    bool m_totemBearer = false;

    Weapon m_stingaBow,
        m_boneShiv,
        m_chompa;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          Yes
// Totem Bearer                     Yes
// Aim Fer Its Eyes                 Yes
// Loadsa Arrows                    Yes
//

} // namespace Bonesplitterz

#endif //ORRUKARROWBOYS_H