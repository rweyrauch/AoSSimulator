/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>


namespace StormcastEternals {

    class CelestarBallista : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        explicit CelestarBallista(Stormhost stormhost);

        ~CelestarBallista() override = default;

    protected:

        void onStartShooting(PlayerId player) override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        void onRestore() override;

    private:

        Weapon m_stormboltsSingle{Weapon::Type::Missile, "Celestar Stormbolts: Single Shot", 36, 1, 3, 3, -2, 1},
                m_stormboltsRapid{Weapon::Type::Missile, "Celestar Stormbolts: Rapid Fire", 18, 4, 5, 3, -2, 1},
                m_sigmariteBlades{Weapon::Type::Melee, "Sigmarite Blades", 1, 4, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bastions of Death                TODO
// Chained Lightning                Yes
// Versatile Weapon                 Yes
//

} // namespace StormcastEternals
