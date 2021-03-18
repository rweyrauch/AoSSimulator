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

namespace GloomspiteGitz {

    class ColossalSquig : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ColossalSquig();

        ~ColossalSquig() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        void onCharged() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int getDamageTableIndex() const;

    private:

        Weapon m_puffSpores,
                m_enormousJaws,
                m_tramplingFeet;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Crazed Charge                    Yes
// Fungoid Squig Explosion          Yes
// Puff Spores                      Yes
// Swallowed Whole                  Yes
//

} // namespace GloomspiteGitz
