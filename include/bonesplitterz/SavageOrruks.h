/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef SAVAGEORRUKS_H
#define SAVAGEORRUKS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{
class SavageOrruks : public Bonesplitterz
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    enum WeaponOption
    {
        Chompa,
        SavageStikka,
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    SavageOrruks();
    ~SavageOrruks() override = default;

    bool configure(int numModels, WeaponOption weapons, bool skullThumper, bool totemBearer);

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

private:

    bool m_thumper = false;
    bool m_totemBearer = false;

    Weapon m_chompa,
        m_stikka,
        m_chompaBoss,
        m_stikkaBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Spirit of Gorkamorka             Yes
// Bone Shield                      Yes
// Thumper                          Yes
// Totem Bearer                     Yes
//

} // namespace Bonesplitterz

#endif //SAVAGEORRUKS_H