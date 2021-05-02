/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class HagQueen : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        HagQueen(Temple temple, Prayer prayer, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~HagQueen() override = default;

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_bladeOfKhaine;

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
