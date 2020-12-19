/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSLORDONDAEMONICMOUNT_H
#define CHAOSLORDONDAEMONICMOUNT_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosLordOnDaemonicMount : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosLordOnDaemonicMount();

        ~ChaosLordOnDaemonicMount() override = default;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_hammer,
                m_hooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Chaos Runeshield                 Yes
// Cursed Warhammer                 Yes
// Fuelled by Carnage               TODO
// The Knights of Chaos             TODO
// Mark of Chaos                    Yes
//

} // SlavesToDarkness

#endif //CHAOSLORDONDAEMONICMOUNT_H