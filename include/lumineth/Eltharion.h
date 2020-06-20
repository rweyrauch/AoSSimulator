/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ELTHARION_H
#define ELTHARION_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class TheLightOfEltharion : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TheLightOfEltharion();

        ~TheLightOfEltharion() override = default;

        bool configure();

    protected:


    private:

        Weapon m_fangsword,
            m_blade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Celenari Blade                   TODO
// Fangsword of Eltharion           TODO
// Searing Darts of Light           TODO
// Spirit Armour                    TODO
// Supreme Swordmaster              TODO
// Unflinching Valour               TODO
//

} // namespace LuminethRealmLords

#endif // ELTHARION_H
