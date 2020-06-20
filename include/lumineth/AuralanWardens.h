/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURALANWARDENS_H
#define AURALANWARDENS_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class AuralanWardens : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuralanWardens();

        ~AuralanWardens() override = default;

        bool configure(int numModels);

    protected:


    private:

        Weapon m_championsBlade,
            m_wardensPike;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Moonfire Flask                   TODO
// Sunmetal Weapons                 TODO
// Wall of Blades                   TODO
// Power of Hysh                    TODO
//

} // namespace LuminethRealmLords

#endif // AURALANWARDENS_H
