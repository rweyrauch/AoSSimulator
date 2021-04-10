/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class ExaltedGreaterDaemonOfKhorne : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ExaltedGreaterDaemonOfKhorne(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        ExaltedGreaterDaemonOfKhorne() = delete;

        ~ExaltedGreaterDaemonOfKhorne() override = default;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onStartHero(PlayerId player) override;

        void onRestore() override;

        void onStartMovement(PlayerId player) override;

    private:

        Weapon  m_lashOfKhorne{Weapon::Type::Missile, "Lash of Khorne", 8, 6, 3, 3, -1, RAND_D3},
                m_mightyAxeOfKhorne{Weapon::Type::Melee, "Mighty Axe of Khorne", 2, 8, 3, 2, -2, 3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drawn in for the Kill            Yes
// The Land Rebels                  Yes
// Rejoice in Exalted Slaughter     Yes
//

} // namespace Khorne
