/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MOURNGUL_H
#define MOURNGUL_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class Mourngul : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Mourngul();

        ~Mourngul() override = default;

        bool configure();

    protected:

        void onWounded() override;

        int getDamageTableIndex() const;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onRestore() override;

    private:

        Weapon m_clawsAndFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// Devourer of Flesh and Souls      TODO
// Ghastly Apparition               TODO


} // namespace Nighthaunt

#endif // MOURNGUL_H
