/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WEBSPINNERSHAMAN_H
#define WEBSPINNERSHAMAN_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz
{

class WebspinnerShaman : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    WebspinnerShaman();
    ~WebspinnerShaman() override = default;

    bool configure(LoreOfTheSpiderFangs lore);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_spiderGodStaff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Touched by the Spider God        No
// Speed of the Spider God          No
//

} // namespace GloomspiteGitz

#endif //WEBSPINNERSHAMAN_H