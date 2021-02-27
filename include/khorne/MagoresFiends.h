/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAGORESFIENDS_H
#define MAGORESFIENDS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class MagoresFiends : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MagoresFiends();

        ~MagoresFiends() override = default;

        bool configure();

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Rerolls chargeRerolls() const override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

    private:

        Weapon m_bellyMaw,
                m_daemonicAxe,
                m_goreaxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Redhand                          Yes
// Gorefists                        Yes
// No Respite                       Yes
// Blood Scent                      Yes
//

} // namespace Khorne

#endif //MAGORESFIENDS_H