/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SCUTTLEBOSSONSPIDER_H
#define SCUTTLEBOSSONSPIDER_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class ScuttlebossOnGiganticSpider : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ScuttlebossOnGiganticSpider();
    ~ScuttlebossOnGiganticSpider() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_spear,
        m_fangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Spider Venom                     Yes
// Wall Crawler                     Yes
// Ride'Em All Down!                No
//

} // namespace GloomspiteGitz

#endif //SCUTTLEBOSSONSPIDER_H