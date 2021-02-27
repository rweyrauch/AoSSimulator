/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODWARRIORS_H
#define BLOODWARRIORS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class BloodWarriors : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodWarriors();

        ~BloodWarriors() override = default;

        bool configure(int numModels, bool pairedGoreax, int numGoreglaives, bool iconBearer);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int braveryModifier() const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

    private:

        bool m_pairedGoreaxe = false;

        Weapon m_goreaxe,
                m_goreaxeChampion,
                m_goreglaive;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// No Respite                       Yes
// Goreaxes                         Yes
// Gorefists                        Yes

} // namespace Khorne

#endif //BLOODWARRIORS_H