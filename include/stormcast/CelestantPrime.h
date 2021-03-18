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

    class CelestantPrime : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        CelestantPrime();

        ~CelestantPrime() override;

        bool configure();

    protected:

        int bearerOfTheWarhammer(const Unit *target);

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_ghalMaraz;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cometstrike Sceptre              Yes
// Retribution from On High         TODO
// Bearer of the Warhammer          Yes
// Orrery of Celestial Fates        TODO
//

} // namespace StormcastEternals
