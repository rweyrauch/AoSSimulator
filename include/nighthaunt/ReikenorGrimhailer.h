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

    class ReikenorTheGrimhailer : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ReikenorTheGrimhailer(Procession procession, Lore lore, bool isGeneral);

        ~ReikenorTheGrimhailer() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_fellreaper,
                m_hoovesAndTeeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Corpse Candles                   TODO
// Frightful Touch                  Yes
// Reaped Like Corn                 Yes
// Wraithstorm                      Yes
//


} // namespace Nighthaunt
