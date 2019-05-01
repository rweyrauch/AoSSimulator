/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIDERWARPARTY_H
#define SPIDERWARPARTY_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class ArachnarokSpiderWithSpiderfangWarparty : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 80;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ArachnarokSpiderWithSpiderfangWarparty();
    ~ArachnarokSpiderWithSpiderfangWarparty() override = default;

    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;

private:

    int getDamageTableIndex() const;

    Weapon m_spiderBows,
        m_chitinousLegs,
        m_monstrousFangs,
        m_crookedSpears;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Voracious Predator               No
// Spider Venom                     No
// Wall Crawler                     No
//

} // namespace GloomspiteGitz

#endif //SPIDERWARPARTY_H