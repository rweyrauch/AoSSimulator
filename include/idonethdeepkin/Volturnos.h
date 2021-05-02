/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class Volturnos : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Volturnos();

        ~Volturnos() override;

    protected:

        void configure(MountTrait trait);

        void onCharged() override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int crestOfTheHighKings(const Unit *target);

    private:

        Weapon m_theAstraSolus,
                m_deepmareJawsTalons,
                m_deepmareTails;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deepmare Horn                    Yes
// The Astra Solus                  Yes
// The Crest of the High King       Yes
// Cealith, the High King's Shield  TODO
// First Among Akhelians            TODO
// Supreme Lord of Tides            TODO
//

} // namespace IdonethDeepkin

