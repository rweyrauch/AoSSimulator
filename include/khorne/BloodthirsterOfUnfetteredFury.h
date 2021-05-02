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

    class BloodthirsterOfUnfetteredFury : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BloodthirsterOfUnfetteredFury() = delete;

        ~BloodthirsterOfUnfetteredFury() override = default;

    protected:

        BloodthirsterOfUnfetteredFury(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onStartHero(PlayerId player) override;

        void onRestore() override;

        void onStartMovement(PlayerId player) override;

    private:

        Weapon  m_lashOfKhorne{Weapon::Type::Missile, "Lash of Khorne", 8, 4, 4, 3, -1, RAND_D3},
                m_mightyAxeOfKhorne{Weapon::Type::Melee, "Mighty Axe of Khorne", 2, 6, 3, 2, -2, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drawn in for the Kill            Yes
// The Land Rebels                  Yes*
// Rejoice in the Slaughter         Yes
//

} // namespace Khorne
