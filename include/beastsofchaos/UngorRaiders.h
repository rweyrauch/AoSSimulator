/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNGORRAIDERS_H
#define UNGORRAIDERS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class UngorRaiders : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    UngorRaiders();
    ~UngorRaiders() override = default;

    bool configure(int numModels, bool brayhorn, bool bannerBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    bool m_brayhorn = false;
    bool m_bannerBearer = false;

    Weapon m_raiderBow,
        m_raiderBowHalfhorn,
        m_jaggedShank;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Vile Invaders                    No
// Braying Anger
//

} // namespace BeastsOfChaos

#endif //UNGORRAIDERS_H