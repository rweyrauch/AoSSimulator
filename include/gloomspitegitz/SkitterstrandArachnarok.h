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

    class SkitterstrandArachnarok : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit SkitterstrandArachnarok(Allegiance allegiance);

        ~SkitterstrandArachnarok() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        size_t getDamageTableIndex() const;

        Weapon m_chitinousLegs,
                m_monstrousFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ambush From Beyond               TODO
// Spider Venom                     Yes
// Wall Crawler                     Yes
//

} // namespace GloomspiteGitz
