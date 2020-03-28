/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TREELORD_H
#define TREELORD_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class Treelord : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Treelord();

        ~Treelord() override = default;

        bool configure();

    protected:

        void onWounded() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int getDamageTableIndex() const;

    private:

        Weapon m_strangleroots,
                m_sweepingBlows,
                m_massiveImpalingTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              TODO
// Impale                           Yes
// Spirit Path                      TODO
//

} // namespace Sylvaneth

#endif //TREELORD_H