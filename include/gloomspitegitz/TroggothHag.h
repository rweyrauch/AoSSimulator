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

    class TroggothHag : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TroggothHag();

        ~TroggothHag() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        void onStartHero(PlayerId player) override;

        void onUnboundSpell(Unit *caster, int castRoll) override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int getDamageTableIndex() const;

    private:

        Weapon m_vomit,
                m_staff,
                m_bulk;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hag Regeneration                 Yes
// Spell-spite                      Yes
// Terrible Stench                  Yes
// Hag Curse                        Yes
//

} // namespace GloomspiteGitz
