/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BESTIGORS_H
#define BESTIGORS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Bestigors : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Bestigors();

        ~Bestigors() override = default;

        bool configure(int numModels, bool brayhorn, bool bannerBearer);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int runModifier() const override;

    private:

        Weapon m_despoilerAxe;
        Weapon m_despoilerAxeGougeHorn;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Despoilers                       Yes
// Beastial Charge                  Yes
//

} // namespace BeastsOfChaos

#endif //BESTIGORS_H