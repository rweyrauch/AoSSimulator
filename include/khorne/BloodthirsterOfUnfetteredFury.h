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

        static int ComputePoints(int numModels);

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

        Weapon m_lashOfKhorne,
                m_mightyAxeOfKhorne;

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
