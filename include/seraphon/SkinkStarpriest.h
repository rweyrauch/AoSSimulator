/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKINKSTARPRIEST_H
#define SKINKSTARPRIEST_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SkinkStarpriest : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkinkStarpriest();

        ~SkinkStarpriest() override = default;

        bool configure(Lore lore);

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_venombolt,
                m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Astral Herald                    Yes
// Serpent Staff                    TODO
// Blazing Starlight                TODO
//

} // namespace Seraphon

#endif //SKINKSTARPRIEST_H