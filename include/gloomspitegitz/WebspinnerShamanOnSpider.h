/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SHAMANONSPIDER_H
#define SHAMANONSPIDER_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz
{

class WebspinnerShamanOnArachnarokSpider : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 160;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    WebspinnerShamanOnArachnarokSpider();
    ~WebspinnerShamanOnArachnarokSpider() override = default;

    int move() const override;

    bool configure(LoreOfTheSpiderFangs lore);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    int getDamageTableIndex() const;

    Weapon m_spiderBows,
        m_spiderGodStaff,
        m_chitinousLegs,
        m_monstrousFangs,
        m_crookedSpears;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Catchweb Spidershrine            No
// Spider Venom                     Yes
// Prophet of the Spider God        No
// Wall Crawler                     Yes
// Venom of the Spider God          No
//

} // namespace GloomspiteGitz

#endif //SHAMANONSPIDER_H