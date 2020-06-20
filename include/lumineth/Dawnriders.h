/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAWNRIDERS_H
#define DAWNRIDERS_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class Dawnriders : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Dawnriders();

        ~Dawnriders() override = default;

        bool configure(int numModels, bool standardBearer);

    protected:


    private:

        Weapon m_guardiansSword,
            m_lance,
            m_hooves;

        bool m_standardBearer = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  TODO
// Deathly Furrows                  TODO
// Sunmetal Weapons                 TODO
// Lances of Dawn                   TODO
// Power of Hysh                    TODO
//

} // namespace LuminethRealmLords

#endif // DAWNRIDERS_H
