/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef SAVAGEBOARBOYS_H
#define SAVAGEBOARBOYS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{

class SavageBoarboys : public Bonesplitterz
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    enum WeaponOption
    {
        Chompa,
        SavageStikka,
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int ComputePoints(int numModels);
    static int EnumStringToInt(const std::string& enumString);

    SavageBoarboys();
    ~SavageBoarboys() override = default;

    bool configure(int numModels, WeaponOption weapons, bool boarThumper, bool totemBearer);

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

    int toSaveModifier(const Weapon *weapon) const override
    {
        // Bone Shield
        int mod = Unit::toSaveModifier(weapon);
        if (!weapon->isMissile())
        {
            mod++;
        }
        return mod;
    }

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

private:

    bool m_thumper = false;
    bool m_totemBearer = false;

    Weapon m_chompa,
        m_stikka,
        m_tusksAndHooves,
        m_chompaBoss,
        m_stikkaBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          Yes
// Totem Bearer                     Yes
// Boarboy Charge                   Yes
// Bone Shield                      Yes
//

} // namespace Bonesplitterz

#endif //SAVAGEBOARBOYS_H