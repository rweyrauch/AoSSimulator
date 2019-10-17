/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIDERRIDERS_H
#define SPIDERRIDERS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class SpiderRiders : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 540;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SpiderRiders();
    ~SpiderRiders() override = default;

    bool configure(int numModels, bool drummers, bool totemBearers);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int braveryModifier() const override;
    int runModifier() const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    bool m_drummers = false;
    bool m_totemBearers = false;

    Weapon m_spiderBow,
        m_crookedSpear,
        m_crookedSpearBoss,
        m_fangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Totem Bearers                    Yes
// Spider Venom                     Yes
// Wall Crawler                     Yes
//

} // namespace GloomspiteGitz

#endif //SPIDERRIDERS_H