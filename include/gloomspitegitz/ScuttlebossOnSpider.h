/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class ScuttlebossOnGiganticSpider : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ScuttlebossOnGiganticSpider();

        ~ScuttlebossOnGiganticSpider() override;

        bool configure();

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int ululatingBattleCry(const Unit *unit);

    private:

        lsignal::slot m_battleCryConnection;

        Weapon m_spear,
                m_fangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Spider Venom                     Yes
// Wall Crawler                     Yes
// Ride'Em All Down!                Partial
//

} // namespace GloomspiteGitz
