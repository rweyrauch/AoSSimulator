/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SORCERESS_H
#define SORCERESS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class Sorceress : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Sorceress();

        ~Sorceress() override = default;

        bool configure(Lore lore);

    protected:


    private:

        Weapon m_witchstaff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood Sacrifice                  TODO
// Word of Pain                     TODO
// Command Underlings               TODO
//

} // namespace CitiesOfSigmar

#endif //SORCERESS_H