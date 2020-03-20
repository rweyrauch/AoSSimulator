/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CONCUSSORS_H
#define CONCUSSORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class Concussors : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Concussors();

        ~Concussors() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

    private:

        Weapon m_stormBlast,
                m_lightningHammer,
                m_clawsAndFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blast to Ashes                   Yes
// Intolerable Damage               Yes
// Sigmarite Shields                Yes
// Storm Blast                      Yes
//

} // namespace StormcastEternals

#endif //CONCUSSORS_H