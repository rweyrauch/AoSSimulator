/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THEDREADPAGEANT_H
#define THEDREADPAGEANT_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class TheDreadPageant : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheDreadPageant();

        ~TheDreadPageant() override = default;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        bool modelIsAlive(const std::string& name) const;

    private:

        Weapon m_bow,
                m_clawAndWeapon,
                m_glaive,
                m_harpoon;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Art of the Myrmidesh             Yes
// Deadlist Procession              Yes
//

} // Slannesh

#endif //THEDREADPAGEANT_H