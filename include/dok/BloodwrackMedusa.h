/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class BloodwrackMedusa : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodwrackMedusa();

        ~BloodwrackMedusa() override = default;

        bool configure(Lore lore);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_bloodwrackStare,
                m_whisperclaw,
                m_tailOfSerpents,
                m_bloodwrackSpear;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodwrack Stare                 Yes
// Enfeebling Foe                   TODO
//

} // namespace DaughtersOfKhaine
