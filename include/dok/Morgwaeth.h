/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class MorgwaethTheBloodied : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MorgwaethTheBloodied();

        ~MorgwaethTheBloodied() override = default;

        bool configure(Prayer prayer);

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_glaive;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Priestess of Khaine              Yes
//    Rune of Khaine                Yes
//    Touch of Death                Yes
// Witchbrew                        Yes
//

} // namespace DaughtersOfKhaine
