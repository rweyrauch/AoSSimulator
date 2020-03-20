/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNGORS_H
#define UNGORS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Ungors : public BeastsOfChaosBase
{
public:

    enum WeaponOptions
    {
        UngorBlade,
        GnarledShortspear
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Ungors();
    ~Ungors() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool brayhorn, bool bannerBearer);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    bool m_brayhorn = false;
    bool m_bannerBearer = false;

    Weapon m_ungorBlade,
        m_ungorBladeHalfhorn,
        m_gnarledShortspear,
        m_gnarledShortspearHalfhorn;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Braying Hatred                   Yes
// Half-shields                     Yes
//

} // namespace BeastsOfChaos

#endif //UNGORS_H