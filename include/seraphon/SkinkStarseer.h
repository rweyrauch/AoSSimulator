/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKINKSTARSEER_H
#define SKINKSTARSEER_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SkinkStarseer : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SkinkStarseer();

        ~SkinkStarseer() override = default;

        bool configure(Lore lore);

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_astralBolt,
                m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cosmic Herald                    Yes
// Astromancer's Staff              TODO
// Control Fate                     TODO
//

} // namespace Seraphon

#endif //SKINKSTARSEER_H