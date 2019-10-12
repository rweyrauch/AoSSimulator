/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef ORRUKMORBOYS_H
#define ORRUKMORBOYS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{
class SavageOrrukMorboys : public Bonesplitterz
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

    SavageOrrukMorboys();
    ~SavageOrrukMorboys() override = default;

    bool configure(int numModels, bool skullThumper, bool totemBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

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

private:

    bool m_thumper = false;
    bool m_totemBearer = false;

    Weapon m_chompaAndShiv,
        m_chompaAndShivBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          Yes
// Totem Bearer                     Yes
// Spirit of Gorkamorka             Yes
// Power of the Beast Spirit        No
//

} // namespace Bonesplitterz

#endif //ORRUKMORBOYS_H