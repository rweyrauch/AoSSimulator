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

    class Wrathmongers : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Wrathmongers() = delete;

        ~Wrathmongers() override;

    protected:

        Wrathmongers(SlaughterHost host, int numModels, int points);

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

        int crimsonHaze(const Unit *attacker, const Model *attackingModel, const Weapon *weapon, const Unit *target);

    private:

        Weapon  m_wrathflails{Weapon::Type::Melee, "Wrath-flails", 2, 4, 4, 3, -1, 1},
                m_wrathflailsMaster{Weapon::Type::Melee, "Wrath-flails", 2, 5, 4, 3, -1, 1};

        lsignal::slot m_hazeSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Furious Assault                  Yes
// Crimson Haze                     Yes
// Bloodfury                        Yes
//

} // namespace Khorne
