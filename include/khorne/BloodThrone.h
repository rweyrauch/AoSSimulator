/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class HeraldOfKhorneOnBloodThrone : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        HeraldOfKhorneOnBloodThrone();

        ~HeraldOfKhorneOnBloodThrone() override = default;

    protected:

        void configure();

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_bladeOfBlood,
                m_hellblades,
                m_gnashingMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Blood Throne                 TODO
// Gorefeast                        TODO
// Decapitating Blow                Yes
//

} // namespace Khorne
