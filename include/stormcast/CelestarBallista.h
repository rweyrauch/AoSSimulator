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

        CelestarBallista();

        ~CelestarBallista() override = default;

    protected:

        void configure();

        void onStartShooting(PlayerId player) override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_stormboltsSingle,
                m_stormboltsRapid,
                m_sigmariteBlades;

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
