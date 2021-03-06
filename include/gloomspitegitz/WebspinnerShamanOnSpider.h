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
#include <gloomspitegitz/GloomspiteSpells.h>

namespace GloomspiteGitz {

    class WebspinnerShamanOnArachnarokSpider : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        WebspinnerShamanOnArachnarokSpider(Allegiance allegiance, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~WebspinnerShamanOnArachnarokSpider() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int catchwebSpidershrine(const Unit *caster);

        int prophetOfTheSpiderGod(const Unit *unit);

    private:

        size_t getDamageTableIndex() const;

        Weapon m_spiderBows,
                m_spiderGodStaff,
                m_chitinousLegs,
                m_monstrousFangs,
                m_crookedSpears;

        lsignal::slot m_shrineConnection,
                m_prophetConnection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Catchweb Spidershrine            Yes
// Spider Venom                     Yes
// Prophet of the Spider God        Yes
// Wall Crawler                     Yes
// Venom of the Spider God          TODO
//

} // namespace GloomspiteGitz
