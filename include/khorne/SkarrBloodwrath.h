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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SkarrBloodwrath() = delete;

        ~SkarrBloodwrath() override = default;

    protected:

        SkarrBloodwrath(SlaughterHost host, bool isGeneral);

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onEndMovement(PlayerId player) override;

    private:

        Weapon m_blades{Weapon::Type::Melee, "Bloodstorm Blades", 3, 5, 2, 3, -1, 1};

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
