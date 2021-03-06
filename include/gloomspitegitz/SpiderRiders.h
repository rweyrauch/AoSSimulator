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

    class SpiderRiders : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        SpiderRiders(Allegiance allegiance, int numModels, bool drummers, bool totemBearers, int points);

        ~SpiderRiders() override = default;

    protected:

        int braveryModifier() const override;

        int runModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_spiderBow,
                m_crookedSpear,
                m_crookedSpearBoss,
                m_fangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Totem Bearers                    Yes
// Spider Venom                     Yes
// Wall Crawler                     Yes
//

} // namespace GloomspiteGitz
