/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIDERFLINGER_H
#define SPIDERFLINGER_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class ArachnarokSpiderWithFlinger : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 160;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 270;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ArachnarokSpiderWithFlinger();
    ~ArachnarokSpiderWithFlinger() override = default;

    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    int getDamageTableIndex() const;

    Weapon m_spiderBows,
        m_flinger,
        m_chitinousLegs,
        m_monstrousFangs,
        m_crookedSpears;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Flinger                          No
// Spider Venom                     Yes
// Wall Crawler                     Yes
//

} // namespace GloomspiteGitz

#endif //SPIDERFLINGER_H