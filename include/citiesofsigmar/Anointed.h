/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ANOINTED_H
#define ANOINTED_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class Anointed : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Anointed();

        ~Anointed() override = default;

        bool configure(Lore lore);

    protected:

        Wounds applyWoundSave(const Wounds &wounds) override;

    private:

        Weapon m_halberd;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blessing of the Ur-Phoenix       Yes
// Witness to Destiny               Yes
// Captain of the Phoenix Guard     TODO
//

} // namespace CitiesOfSigmar

#endif //ANOINTED_H