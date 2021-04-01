/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class VanariStarshardBallistas : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ~VanariStarshardBallistas() override = default;

    protected:

        explicit VanariStarshardBallistas(GreatNation nation);

        void onRestore() override {
            LuminethBase::onRestore();
            m_activatedBlindingBolts = false;
            m_usedBlindingBolts = false;
        }

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onStartShooting(PlayerId player) override;

        void onEndShooting(PlayerId player) override;

    private:

        bool m_activatedBlindingBolts = false;
        bool m_usedBlindingBolts = false;

        Weapon m_bolts{Weapon::Type::Missile, "Starshard Bolts", 30, 2, 3, 3, -2, RAND_D3},
            m_swords{Weapon::Type::Melee, "Arming Swords", 1, 2, 3, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blinding Bolts                   Yes
// Messenger Hawk                   Yes
// Warding Lanterns                 Yes
//

} // namespace LuminethRealmLords

