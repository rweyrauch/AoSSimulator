/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WRATHMONGERS_H
#define WRATHMONGERS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Wrathmongers : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Wrathmongers();

        ~Wrathmongers() override;

        bool configure(int numModels);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        void onModelSlain(Wounds::Source source) override;

        int crimsonHaze(const Unit *attacker, const Model *attackingModel, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_wrathflails,
                m_wrathflailsMaster;

        lsignal::slot m_hazeSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Furious Assault                  Yes
// Crimson Haze                     Partial/TODO
// Bloodfury                        Yes
//

} // namespace Khorne

#endif //WRATHMONGERS_H