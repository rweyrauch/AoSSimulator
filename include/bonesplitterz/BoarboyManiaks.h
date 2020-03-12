/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef BOARBOYMANIAKS_H
#define BOARBOYMANIAKS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{

class SavageBoarboyManiaks : public Bonesplitterz
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels);

    SavageBoarboyManiaks();
    ~SavageBoarboyManiaks() override = default;

    bool configure(int numModels, bool boarThumper, bool totemBearer);

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
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_thumper = false;
    bool m_totemBearer = false;

    Weapon m_chompas,
        m_tusksAndHooves,
        m_chompasBoss;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          Yes
// Totem Bearer                     Yes
// Maniak Fury                      Yes
// Tusker Charge                    Yes
//

} // namespace Bonesplitterz

#endif //BOARBOYMANIAKS_H