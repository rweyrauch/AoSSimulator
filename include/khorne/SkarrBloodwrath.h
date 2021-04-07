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

    class SkarrBloodwrath : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkarrBloodwrath();

        ~SkarrBloodwrath() override = default;

    protected:

        void configure();

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onEndMovement(PlayerId player) override;

    private:

        Weapon m_blades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Slaughterstorm                   Yes
// The Slaughterborn                Yes
// Murderous Paragon                TODO
//

} // namespace Khorne
