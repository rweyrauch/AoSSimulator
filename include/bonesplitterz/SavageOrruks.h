/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef SAVAGEORRUKS_H
#define SAVAGEORRUKS_H

#include "Unit.h"

namespace Bonesplitterz
{
class SavageOrruks : public Unit
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


private:

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
// Spirit of Gorkamorka             No
// Bone Shield                      No
// Thumper                          No
// Totem Bearer                     No
//

} // namespace Bonesplitterz

#endif //SAVAGEORRUKS_H