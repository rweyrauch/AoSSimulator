/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace EldritchCouncil {

    class Loremaster : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Loremaster(bool isGeneral);

        ~Loremaster() override = default;

    protected:

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_greatsword;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deflect Shots                    Yes
// Hand of Glory                    TODO
//

} // namespace EldritchCouncil
