/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GYROCOPTERS_H
#define GYROCOPTERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class Gyrocopters : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            BrimstoneGun,
            SteamGun
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Gyrocopters();

        ~Gyrocopters() override = default;

        bool configure(int numModels, WeaponOption weapons);

    protected:


    private:

        Weapon m_brimstoneGun,
            m_steamGun,
            m_rotorBlades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Steam Gun                        TODO
// Guild Bombs                      TODO
//

} // namespace CitiesOfSigmar

#endif //GYROCOPTERS_H