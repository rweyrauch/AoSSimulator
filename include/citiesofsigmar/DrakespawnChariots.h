/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRAKESPAWNCHARIOTS_H
#define DRAKESPAWNCHARIOTS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class DrakespawnChariots : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        DrakespawnChariots();

        ~DrakespawnChariots() override = default;

        bool configure(int numModels);

    protected:

        void onCharged() override;

    private:

        Weapon m_crossbow,
                m_spear,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Scythed Runners                  Yes
//

} // namespace CitiesOfSigmar

#endif //DRAKESPAWNCHARIOTS_H