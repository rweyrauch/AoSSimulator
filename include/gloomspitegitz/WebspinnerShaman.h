/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <gloomspitegitz/GloomspiteSpells.h>

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

    protected:

        void configure(Lore lore);

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_spiderGodStaff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Touched by the Spider God        Yes
// Speed of the Spider God          Yes
//

} // namespace GloomspiteGitz
