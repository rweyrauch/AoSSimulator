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

namespace GloomspiteGitz {

    class WebspinnerShaman : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        WebspinnerShaman();

        ~WebspinnerShaman() override = default;

        bool configure(LoreOfTheSpiderFangs lore);

    protected:

    private:

        Weapon m_spiderGodStaff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Touched by the Spider God        TODO
// Speed of the Spider God          TODO
//

} // namespace GloomspiteGitz

#endif //WEBSPINNERSHAMAN_H