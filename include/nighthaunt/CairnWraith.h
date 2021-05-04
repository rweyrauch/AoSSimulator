/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class CairnWraith : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        CairnWraith(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~CairnWraith() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_scythe;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightful Touch                  Yes
// Reaped Light Corn                Yes
// Ethereal                         Yes
//

} // namespace Nighthaunt
