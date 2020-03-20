/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AETHERICNAVIGATOR_H
#define AETHERICNAVIGATOR_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class AethericNavigator : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AethericNavigator();

        ~AethericNavigator() override = default;

        bool configure();

    protected:

    private:

        Weapon m_pistol,
                m_zephyrscope;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aethersight                      Yes
// Aetherstorm                      TODO
// Read the Winds                   TODO
//

} // namespace KharadronOverlords

#endif //AETHERICNAVIGATOR_H
