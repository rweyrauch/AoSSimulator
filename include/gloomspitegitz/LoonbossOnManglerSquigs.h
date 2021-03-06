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

    class LoonbossOnManglerSquigs : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LoonbossOnManglerSquigs(Allegiance allegiance, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LoonbossOnManglerSquigs() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        void onWounded() override;

        void onRestore() override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        void onStartHero(PlayerId player) override;

    private:

        size_t getDamageTableIndex() const;

        bool m_eatenRedcapMushroom = false;
        Rerolls m_toHitRerolls = Rerolls::None;
        Rerolls m_toWoundRerolls = Rerolls::None;

        Weapon m_hugeFangFilledGob,
                m_moonCutta,
                m_ballsAndChains,
                m_grotsBashinStikk;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ker-splat!                       Yes
// Redcap Mushrooms                 Yes
// Watch Out!                       Yes
// Bit Da Moon!                     Yes
//

} // namespace GloomspiteGitz
