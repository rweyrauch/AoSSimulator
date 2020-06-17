/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAUGHTERQUEEN_H
#define SLAUGHTERQUEEN_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class SlaughterQueen : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SlaughterQueen();

        ~SlaughterQueen() override = default;

        bool configure(Prayer prayer);

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_bladeOfKhaine,
                m_deathsword;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rune of Khaine                   Yes
// Touch of Death                   Yes
// Dance of Doom                    TODO
// Pact of Blood                    Yes
// Orgy of Slaughter                TODO
//

} // namespace DaughtersOfKhaine

#endif //SLAUGHTERQUEEN_H